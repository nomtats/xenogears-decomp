# Journal Entry: 2026-03-12 - Asset Extraction & Meta-Learnings

## Objective
Extract the required asset binaries (`SLUS_006.64`, `MOVIE.BIN`, `FIELD.BIN`) from a raw PS1 disk image (`Xenogears-disc1.mdf`) to bootstrap the decompilation process.

## Actions Taken
1. Attempted to extract files using standard Linux tools (`7z`, `mdf2iso`, `bchunk`, `binwalk`, `iat`) inside the Docker container.
2. Faced significant struggles recognizing the file layout because PS1 discs often use a non-standard "Mode 2" CD-ROM XA format, and Xenogears specifically hides its overlay files in custom LZSS compressed archives mapped directly to CD sectors.
3. Paused to re-evaluate the approach after standard utilities completely failed.
4. Searched the repository for hints about `FIELD.BIN` and discovered `tools/scripts/extract_overlays.py`, a utility explicitly provided by the project authors to solve this exact problem.
5. Discovered the `.mdf` format had 2448-byte sectors (with 96-byte subchannel data), which broke the python extraction tool. Wrote a small script (`mdf_to_bin.py`) to strip the 96 bytes and convert it to a standard 2352-byte `.bin` file.
6. Successfully executed `extract_overlays.py` to acquire all missing overlays natively.

## Meta-Learnings
This exhaustive extraction process highlighted critical flaws in the AI's autonomous approach. Two new meta-learnings have emerged:
1. **Examine Available Tools First:** The solution was sitting in the repository the entire time. Before attempting to brute-force a problem with external tools, always search the codebase (e.g., `tools/` or `scripts/` directories) to see if a custom solution already exists.
2. **Pause When Struggling:** If a tool fails 3 or 4 times, do not stubbornly barrage the environment with alternative CLI tools. Stop, step back, re-evaluate assumptions, and discuss the roadblock with the USER to formulate a better strategy.
