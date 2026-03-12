# Journal Entry: 2026-03-12 - Repository Tools Survey

## Objective
Proactively survey the `tools/` and `tools/scripts/` directories to understand exactly what custom utilities the repository developers have already provided. This satisfies the new `Examine All Available Tools First` directive and ensures we don't rebuild wheels or miss crucial context like we did with the LZSS extraction.

## Analyzed Directories
- `/tools/`
- `/tools/scripts/`

## Tool Index

### Core Compiler & Build Tools
*   `tools/gcc-2.6.0-psx/`: A specific older version of GCC required for PS1 PsyQ code generation.
*   `tools/gcc-2.7.2-psx/`: Another specific PsyQ GCC version, likely for different modules or later SDK versions.
*   `tools/gcc-2.7.2-cdk-psx/`: A third PsyQ compiler variant.
*   `tools/maspsx/`: A critical python tool (Macro Assembler PSX) that translates the proprietary PsyQ assembly syntax output by the old GCC into standard GNU MIPS assembly so `mips-linux-gnu-as` can assemble it.
*   `tools/objdiff/`: The diffing utility used to compare our newly compiled `.o` files byte-for-byte against the original game disc extractions.
*   `tools/splat_ext/`: Extensions for the `splat` tool (the primary utility used to split the original binaries into `.c`/`.s` text files).

### Helper Scripts (`tools/scripts/`)
1.  **`extract_overlays.py`**:
    *   **Purpose**: Extracts and decompresses custom LZSS-compressed overlay binaries directly from raw PlayStation Mode 2 sectors.
    *   **Input**: A raw 2352-byte/sector `.bin` track.
    *   **Output**: The decompressed overlay files (`FIELD.BIN`, `MOVIE.BIN`, etc.).
2.  **`get_yaml_target.py`**:
    *   **Purpose**: Parses a `splat` YAML configuration file to extract the target path for a specific split.
    *   **Input**: A path to a `configs/*.yaml` file.
    *   **Output**: The cleaned target bin filename.
3.  **`ghidra_import_splat_symbols.py`**:
    *   **Purpose**: A script meant to be run directly inside the Ghidra reverse-engineering tool. It parses our `splat` symbol definition files (e.g. `symbol_addrs.txt`) and formally defines functions and labels inside the Ghidra project database for easier static analysis.
4. **`strip_mdf.py`** & **`ci_extract_assets.sh`**:
    *  **Purpose**: The automated scripts we wrote to safely strip Alcohol 120% padding and perform a full hands-free disc extraction.

## Conclusion
The repository provides a complete end-to-end toolchain. Any compilation should strictly pass through `maspsx`, and any future Ghidra integration during reverse engineering can utilize the provided symbol import script. We have fully cataloged the available utilities and will consult this list before downloading external binaries.
