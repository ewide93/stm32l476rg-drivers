import time
import sys
import subprocess
import logging
from pathlib import Path
from dataclasses import dataclass
from typing import Optional
from pyocd.core.helpers import ConnectHelper
from pyocd.core.target import Target
from pyocd.core.session import Session
from pyocd.core.soc_target import SoCTarget
from pyocd.debug.elf.decoder import ElfSymbolDecoder, SymbolInfo
from elftools.elf.elffile import ELFFile
from elftools.elf.sections import Section


@dataclass
class SectionInfo:
    size: int
    start_address: int
    # TODO: Populate with other useful data


class Debugger:
    def __init__(self, elf_file: Path, *, debug: bool = False) -> None:
        self._logger = self._init_logger(debug)
        if elf_file.exists() and elf_file.suffix == ".elf":
            self._elf = ELFFile.load_from_path(elf_file)
            self._elf_name = elf_file.name
            self._elf_decoder = ElfSymbolDecoder(self._elf)
            self._logger.info(f"Loaded file: {elf_file.name}")
        else:
            self._logger.error(f"Failed to load {elf_file}")
            sys.exit(-1)
        self._elf_sections = self._read_elf_sections()
        self.debug_session, self.target = self._init_debug_session()

    def _init_logger(self, debug: bool) -> logging.Logger:
        logger = logging.getLogger("debugger")
        handler = logging.StreamHandler(sys.stdout)
        fmt = logging.Formatter("%(asctime)s - [%(levelname)s]: %(message)s")
        handler.setFormatter(fmt)
        logger.addHandler(handler)
        logger.setLevel(logging.DEBUG if debug else logging.INFO)
        return logger

    def _init_debug_session(self) -> tuple[Session, SoCTarget]:
        session = ConnectHelper.session_with_chosen_probe()
        if session is None:
            self._logger.error("Failed to create debug session!")
            sys.exit(-1)
        target = session.target
        if target is None:
            self._logger.error("Failed to initialize SoC target!")
            sys.exit(-1)
        return (session, target)

    def _read_elf_sections(self) -> dict[str, SectionInfo]:
        sections = {}
        for section in self._elf.iter_sections():
            if isinstance(section, Section) and section.name != "":
                sections.update({section.name: SectionInfo(section.data_size, section.header["sh_addr"])})
        return sections

    def get_section_info(self, section_name: str) -> SectionInfo:
        if section_name not in self._elf_sections:
            err_msg = f"Section {section_name} not found in {self._elf_name}"
            raise KeyError(err_msg) from None
        return self._elf_sections[section_name]

    def get_symbol_info(self, symbol_name: str) -> SymbolInfo:
        symbol_info = self._elf_decoder.get_symbol_for_name(symbol_name)
        if isinstance(symbol_info, SymbolInfo):
            return symbol_info
        else:
            err_msg = f"Symbol {symbol_name} not found in {self._elf_name}"
            raise KeyError(err_msg) from None

    def wait_for_breakpoint(self, timeout_s: Optional[float] = None) -> bool:
        breakpoint_hit = False
        if not self.debug_session.is_open:
            self._logger.warning("Debug session needs to be opened before interacting with target!")
        else:
            start_time = time.perf_counter()
            self._logger.debug("Waiting for target to hit breakpoint...")
            while not breakpoint_hit:
                state = self.target.get_state()
                if state == Target.State.HALTED and self.target.get_halt_reason() == Target.HaltReason.BREAKPOINT:
                    self._logger.debug(f"Breakpoint hit after {round(time.perf_counter() - start_time, 3)} [s]")
                    breakpoint_hit = True
                    break
                else:
                    if timeout_s is not None and time.perf_counter() - start_time >= timeout_s:
                        self._logger.debug("Timed out waiting for target to hit breakpoint!")
                        break
                time.sleep(0.05)
        return breakpoint_hit


def main() -> None:
    root_path = Path(__file__).parent.joinpath("..").resolve()
    elf_path = root_path.joinpath("stm32l476rg/build/out/STM32L476RG.elf")
    dbg = Debugger(elf_path, debug=True)
    with dbg.debug_session:
        dbg.target.reset(Target.ResetType.HW)

        hit = dbg.wait_for_breakpoint(timeout_s=2.5)
        print(hit)
        t_start = time.perf_counter()
        print(dbg.target.read32(dbg.get_symbol_info("DummyCnt").address))
        t_after_read = time.perf_counter()
        print(f"Read 32-bit RAM variable in {(t_after_read - t_start) * 1000} [ms]")


if __name__ == "__main__":
    main()
