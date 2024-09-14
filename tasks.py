from invoke import task
from invoke.context import Context
from pathlib import Path
from scripts.binutils import Binutils


PATHS = {
    "build_dir": Path("stm32l476rg/build"),
    "obj_dir": Path("stm32l476rg/build/obj"),
    "out_dir": Path("stm32l476rg/build/out"),
    "elf": Path("stm32l476rg/build/out/STM32L476RG.elf"),
    "makefile": Path("stm32l476rg/build/Makefile"),
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
    text, data, bss = Binutils.parse_size(output.stdout)
    flash_used = text + data
    flash_used_percent = round(flash_used / DEVICE_INFO["flash"]["bytes"] * 100, 2)
    ram_used = data + bss
    ram_used_percent = round(ram_used / DEVICE_INFO["ram"]["bytes"] * 100, 2)
    print(f"Flash used: {flash_used_percent}% ({flash_used} B)")
    print(f"RAM used: {ram_used_percent}% ({ram_used} B)")


@task()
def nm(ctx: Context) -> None:
    """List the symbols in the firmware image."""
    output = ctx.run(f"{Binutils.COMMANDS["nm"]} {PATHS['elf']}", hide=True)
    print(Binutils.parse_nm(output.stdout))


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
