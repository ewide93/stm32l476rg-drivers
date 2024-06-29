import sys
from invoke import task
from invoke.context import Context
from pathlib import Path

PATHS = {
    "build_dir": Path("build"),
    "obj_dir": Path("build/obj"),
    "out_dir": Path("build/out"),
    "elf": Path("build/out/STM32L476RG.elf"),
    "makefile": Path("build/Makefile"),
    "src_dir": Path("src"),
    "inc_dir": Path("inc"),
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
