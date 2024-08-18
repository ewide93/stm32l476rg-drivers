import sys
import re
from invoke import task
from invoke.context import Context
from pathlib import Path
from typing import NamedTuple, List
from enum import IntEnum


class Symbol(NamedTuple):
    name: str
    address: int
    size: int
    symbol_type: str
    src_file: str
    line_num: int


PATHS = {
    "build_dir": Path("build"),
    "obj_dir": Path("build/obj"),
    "out_dir": Path("build/out"),
    "elf": Path("build/out/STM32L476RG.elf"),
    "makefile": Path("build/Makefile"),
    "src_dir": Path("src"),
    "inc_dir": Path("inc"),
}

DEVICE_INFO = {
    "flash": {"bytes": 1048576, "KiB": 1024, "MiB": 1},
    "ram": {"bytes": 131072, "KiB": 128},
}

BINUTILS = {
    "size": "arm-none-eabi-size",
    "nm": "arm-none-eabi-nm",
}


def parse_size(stdout: str) -> tuple:
    """Parse output from arm-none-eabi-size utility returning
    a tuple containing the sizes of the text,
    data & bss sections in bytes."""
    text, data, bss = stdout.split("\n")[1].split()[:3]
    return (int(text), int(data), int(bss))


def parse_nm(stdout: str) -> List[Symbol]:
    """Parse output from arm-none-eabi-nm utility returning
    a list of tuples containing symbol address,
    symbol size, symbol type, symbol name, source file for symbol
    and the line number in the source file where the symbol is defined."""
    symbols = []
    for line in stdout.removesuffix("\n").split("\n"):
        address, size, symbol_type, name, path_and_line_num = line.split()
        _, file_path, line_num = path_and_line_num.split(":")
        regex = re.compile(r"^.+\/([a-zA-Z_]+.c)$")
        match = regex.match(file_path)
        if match:
            src_file = match.group(1)
            symbols.append(
                Symbol(
                    name,
                    hex(int(address, 16)),
                    int(size, 16),
                    symbol_type,
                    src_file,
                    int(line_num),
                )
            )
    return sorted(symbols, key=lambda sym: sym.src_file)

def symbol_type_to_str(symbol: Symbol) -> str:
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

@task()
def build(ctx: Context) -> None:
    """Build the firmware image."""
    with ctx.cd(PATHS["build_dir"]):
        ctx.run("make")


@task()
def clean(ctx: Context) -> None:
    """Clean build artifacts."""
    with ctx.cd(PATHS["build_dir"]):
        ctx.run("make clean")


@task()
def flash(ctx: Context) -> None:
    """Flash the firmware image to the device."""
    with ctx.cd(PATHS["build_dir"]):
        ctx.run("make flash")


@task()
def size(ctx: Context) -> None:
    """Print the firmware image size."""
    if not PATHS["elf"].exists():
        print(f"Path {PATHS['elf']} does not exist.")
    output = ctx.run(f"{BINUTILS['size']} {PATHS['elf']}", hide=True)
    text, data, bss = parse_size(output.stdout)
    flash_used = text + data
    flash_used_percent = round(flash_used / DEVICE_INFO["flash"]["bytes"] * 100, 2)
    ram_used = data + bss
    ram_used_percent = round(ram_used / DEVICE_INFO["ram"]["bytes"] * 100, 2)
    print(f"Flash used: {flash_used_percent}% ({flash_used} B)")
    print(f"RAM used: {ram_used_percent}% ({ram_used} B)")


@task()
def nm(ctx: Context) -> None:
    """List the symbols in the firmware image."""
    flags = "--print-size --size-sort --reverse-sort --line-numbers"
    output = ctx.run(f"{BINUTILS['nm']} {flags} {PATHS['elf']}", hide=True)
    symbols = parse_nm(output.stdout)
    src_files = []
    for symbol in symbols:
        if symbol.src_file not in src_files:
            src_files.append(symbol.src_file)
            print("\n" + f"-- Source file: {symbol.src_file} ".ljust(88, "-"))
        print(f"Name: {symbol.name}")
        print(f"Address: {symbol.address}".ljust(22, " "), end="")
        print(f"Size: {symbol.size}".ljust(10, " "), end="")
        print(f"Line: {symbol.line_num}".ljust(10, " "), end="")
        print(f"Type: {symbol_type_to_str(symbol)}\n")
