# Journal Entry: 2026-03-12 - System Requirements Analysis

## Objective
Analyze the current build system, dependencies, and environment required to compile and verify the Xenogears decompilation.

## Learnings & Observations
1. **Toolchain Architecture**:
   - The PS1 compiled binaries require custom old GCC versions (2.6.0 and 2.7.2) which are present in the `tools/` directory.
   - However, standard GNU `binutils` configured for MIPS (specifically `binutils-mips-linux-gnu` providing `as`, `ld`, and `objcopy`) are required to assemble and link the code. 
   - A MIPS pre-processor `cpp-mips-linux-gnu` is also used for macros.
   - Since we are on a Mac, these tools are not natively available. While Homebrew has MIPS toolchains, to ensure 1:1 parity with the GitHub Actions CI (which uses `ubuntu-latest`), using a local **Docker container** is the most robust solution.

2. **Bootstrapping the Disassembly**:
   - The `.s` assembly files are currently missing. This is intentional; decompilation projects require you to provide your own legal copy of the original binary to avoid copyright infringement.
   - The CI `.yml` accesses the files from a secret URL.
   - The `Makefile` relies on `make setup` / `make generate`, which invokes `splat` (a Python tool) to parse `config/slus_006.64.yaml` and split the target binaries into `src/` stubs and `asm/` instructions.
   - **Crucial Limitation found**: The process will strictly fail right now because the `disc/` directory is completely empty except for `.gitkeep`.

## Next Steps
1. Request the USER to place the original un-headered binaries (`SLUS_006.64`, `MOVIE.BIN`, `FIELD.BIN`) into the `disc/` directory, as the AI system cannot autonomously acquire copyrighted assets.
2. Build a local Docker container (with Ubuntu, Python 3, and mips toolchains) designed to run as a daemon. 
3. Add a "Docker runner" skill to the `skills/` directory so the AI orchestrator knows how to transparently pass `make` commands to this container.
