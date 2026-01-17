import re
from typing import NamedTuple, Optional
from pathlib import Path


class Symbol(NamedTuple):
    name: str
    address: int
    size: int
    symbol_type: str
    src_file: str
    line_num: int


class Binutils:

    COMMANDS = {
        "size": "arm-none-eabi-size",
        "nm": "arm-none-eabi-nm --print-size --size-sort --reverse-sort --line-numbers",
    }

    @classmethod
    def symbol_type_to_str(cls, symbol: Symbol) -> str:
        """Return a string representation of the given symbol type."""
        match symbol.symbol_type:
            case "A":
                return "Absolute"
            case "B" | "b":
                return "Zero initialized data"
            case "C" | "c":
                return "Common"
            case "D" | "d":
                return "Initialized data"
            case "G" | "g":
                return "Small initialized data"
            case "I" | "i":
                return "Indirect reference"
            case "N":
                return "Debug"
            case "n":
                return "Read-only data"
            case "p":
                return "Stack unwind"
            case "S" | "s":
                return "Small zero initialized data"
            case "T" | "t":
                return "Code"
            case "U":
                return "Undefined"
            case "u":
                return "Unique global"
            case "W" | "w" | "V" | "v":
                return "Weak"
            case "?" | _:
                return "Unknown"

    @classmethod
    def parse_nm(cls, stdout: str, file: Optional[str]) -> str:
        """Parse output from arm-none-eabi-nm utility."""
        symbols: list[Symbol] = []
        for line in stdout.removesuffix("\n").split("\n"):
            try:
                address, size, symbol_type, name, path_and_line_num = line.split()
                file_path, line_num = path_and_line_num.split(":")
                regex = re.compile(r"^.+\/([a-zA-Z_]+.c)$")
                match = regex.match(file_path)
                if match:
                    src_file = match.group(1)
                    symbols.append(
                        Symbol(
                            name,
                            int(address, 16),
                            int(size, 16),
                            symbol_type,
                            src_file,
                            int(line_num),
                        )
                    )
            except ValueError:
                continue
        src_files = []
        parsed_output = ""
        for symbol in sorted(symbols, key=lambda sym: sym.src_file):
            if file is not None and symbol.src_file != file:
                continue
            if symbol.src_file not in src_files:
                src_files.append(symbol.src_file)
                parsed_output += (
                    "\n" + f"-- Source file: {symbol.src_file} ".ljust(88, "-") + "\n"
                )
            parsed_output += f"Name: {symbol.name}\n"
            parsed_output += f"\tAddress: {hex(symbol.address)}".ljust(22, " ")
            parsed_output += f"Size: {symbol.size}".ljust(10, " ")
            parsed_output += f"Line: {symbol.line_num}".ljust(10, " ")
            parsed_output += f"Type: {cls.symbol_type_to_str(symbol)}\n"

        if file is not None and parsed_output == "":
            return f"No symbol data found for file: {file}"
        else:
            return parsed_output

    @staticmethod
    def parse_size(stdout: str) -> tuple:
        """Parse output from arm-none-eabi-size utility returning
        a tuple containing the sizes of the text,
        data & bss sections in bytes."""
        text, data, bss = stdout.split("\n")[1].split()[:3]
        return (int(text), int(data), int(bss))
