# AI Decompilation Journal: Master Log

This document tracks the ongoing activities, decisions, and progress of the AI-assisted decompilation workflow. It serves as the primary entry point for any new agent session to quickly understand recent context without needing to parse individual files.

## 🟢 Current Status
- **Phase 4 (Sustained Manual Decompilation)** is active. We are executing the "Jigsaw Strategy," starting by decompiling highly-referenced "True Leaf" nodes (functions with 0 callees).
- We have successfully cleared our first edge targets: `SetTransMatrix`, `SetRotMatrix`, and `PCclose` by matching inline Coprocessor 2 and Exception assembly.
- Our immediate next steps are to continue solving the top heavily-referenced leaf targets from `ai_workflow/analysis/call_graph_report.md` (e.g. `NormalClip`, `func_8008CF3C`, `memcpy`) to progressively shrink the "unknown" footprint underlying all major systems.

## 📋 Project Roadmap & Next Steps
### Phase 1: Environment Setup (Completed)
- [x] Establish meta-workflow and AI directives
- [x] Configure Docker build environment
- [x] Extract raw PS1 assets (`SLUS_006.64`, `FIELD.BIN`, etc.) using newly discovered tools

### Phase 2: Repository Bootstrapping (Completed)
- [x] Run `make setup` inside the container to use `splat` to carve the extracted binaries into `src/` (C) and `asm/` (assembly).
- [x] Run `make build` inside the container to compile the codebase securely using PsyQ GCC and `maspsx`.
- [x] Run `make report` / `make check` to verify the baseline build perfectly matches the original disc bytes using `objdiff`.

### Phase 3: Manual Decompilation & Strategy (Completed)
- [x] Analyze `build/progress.json` to map decompilation targets.
- [x] Attempt manual decompilation of small functions (e.g., in `system/` or `psyq/`) to familiarize oneself with PsyQ optimizations and compiler quirks.
- [x] Update header files (`types.h`, `common.h`) to establish data structures.

### Phase 4: Sustained Manual Decompilation (In Progress)
- [x] Use `generate_call_graph.py` iteratively to systematically identify and decompile heavily referenced subsystems (e.g., `system/sound.c`).
- [x] Discover and document compiler quirks, memory layout, and branch delay slot nuances for PsyQ GCC 2.6.0/2.7.2.
- [ ] Achieve 100% decompilation for high-priority, near-complete submodules:
  - **`libspu`**: 97.7% complete (1 unmatched function: `D_8001946C`, 7 ASM lines).
  - **`libsn`**: 12.5% complete (7 unmatched, very similar SN systems I/O routines like `PClseek`, `PCopen`, 5-55 ASM lines).
  - **`libcard`**: 14.3% complete (6 unmatched, tiny functions 6-23 ASM lines).
  - **`libc`**: 33.3% complete (8 unmatched standard library routines like `memcpy`, `bzero`).
  - **`system/libarchive`**: 77.8% complete (6 unmatched, larger functions up to 352 ASM lines, but near completion).
- [ ] Continue mapping out and decompiling remaining critical sub-systems requiring manual intervention.

### Phase 5: Autonomous Orchestration (Next)
- [ ] Draft `scripts/auto_decomp.py` to act as the central AI orchestrator for simpler leaf functions.
- [ ] Implement the continuous compilation feedback loop (Pick un-matched `.s` -> Prompt AI for `.c` -> Compile -> Evaluate Diff/Fix -> Repeat).
- [ ] Plumb the feedback loop to automatically log successful compiler tricks and heuristics back into `knowledge_base.json`.

## 📖 Detailed Log Entries

| Date | Entry Target | Summary | Link |
| :--- | :--- | :--- | :--- |
| 2026-03-12 | Meta-Workflow Initialization | Established the self-improvement loop, knowledge base structure, and journaling system. | [2026-03-12_1734_workflow_initialization.md](./2026-03-12_1734_workflow_initialization.md) |
| 2026-03-12 | System Requirements Analysis | Analyzed `Makefile` and CI dependencies. Identified requirement for Docker and original PS1 binaries. | [2026-03-12_1741_system_analysis.md](./2026-03-12_1741_system_analysis.md) |
| 2026-03-12 | Asset Extraction & Meta-Learnings | Extracted PS1 disc using repository python scripts. Adopted new 'Pause When Struggling' directive. | [2026-03-12_1820_asset_extraction_learnings.md](./2026-03-12_1820_asset_extraction_learnings.md) |
| 2026-03-12 | Repository Tools Survey | Executed the Extract Tools First directive. Cataloged all scripts and utilities in `tools/`. | [2026-03-12_1848_tools_survey.md](./2026-03-12_1848_tools_survey.md) |
| 2026-03-12 | Repository Bootstrapping | Successfully carved assets, resolved submodule build issues, and generated `objdiff` hashes. | [2026-03-12_2251_bootstrapping_success.md](./2026-03-12_2251_bootstrapping_success.md) |
| 2026-03-12 | Manual Decompilation Execution | Replaced `INCLUDE_ASM` for `func_80021FB8.s` using `u8*` casting to target `sb` offset. Validated build. | [2026-03-12_2319_first_manual_decomp.md](./2026-03-12_2319_first_manual_decomp.md) |
| 2026-03-12 | Fast Incremental Builds | Used `make build` to avoid clean-build cache tainting from `make report`. Matched additional leaf functions. | [2026-03-12_2334_fast_incremental_builds.md](./2026-03-12_2334_fast_incremental_builds.md) |
| 2026-03-12 | Target Reference Scripting & GCC Register Overrides | Rewrote `find_target.py` to sort unmatched functions by caller density. Matched highest target via `asm("reg")` explicit GCC extensions. | [2026-03-12_2348_gcc_register_casting.md](./2026-03-12_2348_gcc_register_casting.md) |
| 2026-03-13 | Meaningful Symbol Renaming | Documented the process of deciphering abstract memory addresses (`func_800295D8`) into cohesive functions (`ArchiveRead`) and syncing labels globally using scripts. | [2026-03-13_0012_meaningful_renaming.md](./2026-03-13_0012_meaningful_renaming.md) |
| 2026-03-13 | SPU Sound Handle Error Decompilation | Decompiled `SoundHandleError` and recovered system error flags triggering WDS and SPU memory reloads over DMA. | [2026-03-13_0025_sound_handle_error.md](./2026-03-13_0025_sound_handle_error.md) |
| 2026-03-13 | Data Block Mappings & SEDS Recovery | Discovered fallback default sound data structures by reading `asm/slus_006.64/data` magics and renaming them globally; decompiled SEDS playback recovery function. | [2026-03-13_0055_sound_seds_recovery.md](./2026-03-13_0055_sound_seds_recovery.md) |
| 2026-03-13 | WDS Allocation and Delay Slots | Resolved GCC 2.6.0 instruction reordering involving MIPS branch delay slots and `do-while` linked-list matching. Exploded `AudioElement` struct black-box parameters recursively via initialization decompilation. | [2026-03-13_0130_sound_wds_allocation_delay_slots.md](./2026-03-13_0130_sound_wds_allocation_delay_slots.md) |
| 2026-03-13 | Flat Goto Block Manipulation vs GCC | Discovered how to force GCC 2.7.2 to emit explicit true-branches (`bnez`, `beq`) spanning distant blocks, and realized `asm/` is untracked by `git` preventing manual deletions. | [2026-03-13_0140_goto_block_manipulation.md](./2026-03-13_0140_goto_block_manipulation.md) |
| 2026-03-13 | Static Call Graph Analysis | Removed `find_target.py`, replacing it with `generate_call_graph.py` which tracks R_MIPS_26 relocations and groups stats by subsystem to accurately map "True Leaf" nodes. | [2026-03-13_1215_call_graph_generation.md](./2026-03-13_1215_call_graph_generation.md) |
| 2026-03-13 | PsyQ SDK Inline Assembly | Decompiled highly referenced SDK functions (`SetRotMatrix`, `SetTransMatrix`, `PCclose`) using MIPS `break` and `__asm__ volatile` coping with maspsx parser issues involving integer bases. | [2026-03-13_1416_psyq_sdk_inline_asm.md](./2026-03-13_1416_psyq_sdk_inline_asm.md) |
