# AI Decompilation Journal: Meaningful Renaming

## Summary
To make the decompiled assembly genuinely human-readable, we focused on decompiling a highly-referenced function (`func_800295D8`), determining its real purpose, and renaming it globally to `ArchiveRead`.

## Key Activities
- Targeted the most called non-library function (`func_800295D8` with 71 references) and successfully matched it byte-for-byte in C.
- Deduced through context (wrapped function `ArchiveReadFile`, variables manipulated, and parameter pass-through) what the symbol meant.
- `func_800295D8` logically became `ArchiveRead`.
- `D_8004FE18` logically became `g_ArchiveSavedOffset`.
- Replaced the arbitrary `param3` and `param4` labels with `audioChannel` and `readModeFlags`.

## Key Discoveries
- Renaming symbols in the codebase requires careful synchronization. Modifying just `.c` and `.h` files is insufficient because the `splat` configuration structure rely heavily on `.yaml` metadata and symbol address lists like `config/symbol_addrs.slus_006.64.txt`. 
- When we renamed items arbitrarily, it triggered immediate linker errors (`undefined reference`) because `.s` object layers were still requesting the original function labels from `yaml`.
- By universally rewriting labels across `src/`, `include/`, `asm/`, `config/`, and `yaml/` files, we proved we can reliably refactor meaningless addresses into high-level, human-readable abstractions.
- We modified `find_target.py` to bake in `--exclude-filter="psyq"` to automatically skip SDK components.
