import sys
import re
from invoke.tasks import task
from invoke.context import Context
from pathlib import Path
from scripts.binutils import Binutils
from typing import Optional


PATHS = {
    "build_dir": Path("stm32l476rg/build"),
    "obj_dir": Path("stm32l476rg/build/obj"),
    "out_dir": Path("stm32l476rg/build/out"),
    "elf": Path("stm32l476rg/build/out/STM32L476RG.elf"),
    "makefile": Path("stm32l476rg/build/Makefile"),
    "test_dir": Path("stm32l476rg/test"),
    "test_build_dir": Path("stm32l476rg/test/build"),
    "test_results": Path("stm32l476rg/test/build/results.txt"),
}

DEVICE_INFO = {
    "flash": {"bytes": 1048576, "KiB": 1024, "MiB": 1},
    "ram": {"bytes": 131072, "KiB": 128},
}


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
    output = ctx.run(f"{Binutils.COMMANDS["size"]} {PATHS['elf']}", hide=True)
    if output is not None:
        text, data, bss = Binutils.parse_size(output.stdout)
        flash_used = text + data
        flash_used_percent = round(flash_used / DEVICE_INFO["flash"]["bytes"] * 100, 2)
        ram_used = data + bss
        ram_used_percent = round(ram_used / DEVICE_INFO["ram"]["bytes"] * 100, 2)
        print(f"Flash used: {flash_used_percent}% ({flash_used} B)")
        print(f"RAM used: {ram_used_percent}% ({ram_used} B)")


@task()
def nm(ctx: Context, file: Optional[str] = None) -> None:
    """List the symbols in the firmware image."""
    output = ctx.run(f"{Binutils.COMMANDS["nm"]} {PATHS['elf']}", hide=True)
    if output is not None:
        print(Binutils.parse_nm(output.stdout, file))


@task()
def lint(ctx: Context) -> None:
    """Run the cppcheck linter."""
    with ctx.cd(PATHS["build_dir"]):
        print("Running static code analysis...")
        ctx.run("make lint")
    print("Static code analysis complete!")
    with open(PATHS["build_dir"].joinpath("cppcheck_output.txt"), mode="r") as file:
        line_cnt = 0
        print("-- Results: cppcheck ".ljust(88, "-"))
        for line in file:
            print(line)
            line_cnt += 1
    if line_cnt == 0:
        print("No errors detected!")


def parse_test_results(results: Path) -> int:
    failed_tests = []
    test_cnt = 0
    fail_cnt = 0
    skip_cnt = 0
    return_code = 0
    with results.open() as istream:
        regex = re.compile(r"^([0-9]+) Tests ([0-9]+) Failures ([0-9]+) Ignored$")
        for line in istream:
            match = regex.match(line.strip())
            if match:
                test_cnt += int(match.group(1))
                fail_cnt += int(match.group(2))
                skip_cnt += int(match.group(3))
            if ":FAIL:" in line:
                file, line_num, test_case, _, msg = line.strip().split(":")
                failed_tests.append(f"{test_case} at line {line_num} in {file}:\n\t{msg}")
    pass_cnt = test_cnt - fail_cnt - skip_cnt
    print("\n-- Results: Unit tests ".ljust(88, "-"))
    parsed_results = f"Passed: {pass_cnt} ({round((pass_cnt / test_cnt) * 100, 1)}%)\t"
    parsed_results += f"Failed: {fail_cnt} ({round((fail_cnt / test_cnt) * 100, 1)}%)\t"
    parsed_results += f"Skipped: {skip_cnt} ({round((skip_cnt / test_cnt) * 100, 1)}%)\t\n"
    print(parsed_results)
    if failed_tests:
        return_code = -1
        print("-- Failed tests: ".ljust(88, "-"))
        for failed_test in failed_tests:
            print(failed_test)
        print("\n")
    return return_code


@task
def test(ctx: Context) -> None:
    """Build & run unit tests."""
    with ctx.cd(PATHS["test_dir"]):
        ctx.run("make")
    return_code = parse_test_results(PATHS["test_results"])
    with ctx.cd(PATHS["test_dir"]):
        ctx.run("make clean")
    sys.exit(return_code)
