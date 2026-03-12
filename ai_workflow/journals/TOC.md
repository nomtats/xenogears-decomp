# AI Decompilation Journal: Master Log

This document tracks the ongoing activities, decisions, and progress of the AI-assisted decompilation workflow. It serves as the primary entry point for any new agent session to quickly understand recent context without needing to parse individual files.

## 🟢 Current Status
- **Bootstrapping Complete. Orchestration Next.** All original assets have been carved by `splat`, standard builds compile successfully via PsyQ tools/maspsx, and verification passes perfectly against the original binaries. We are now pivoting to the creation of the continuous AI evaluation loop (`auto_decomp.py`).

## 📋 Project Roadmap & Next Steps
### Phase 1: Environment Setup (Completed)
- [x] Establish meta-workflow and AI directives
- [x] Configure Docker build environment
- [x] Extract raw PS1 assets (`SLUS_006.64`, `FIELD.BIN`, etc.) using newly discovered tools

### Phase 2: Repository Bootstrapping (Completed)
- [x] Run `make setup` inside the container to use `splat` to carve the extracted binaries into `src/` (C) and `asm/` (assembly).
- [x] Run `make build` inside the container to compile the codebase securely using PsyQ GCC and `maspsx`.
- [x] Run `make report` / `make check` to verify the baseline build perfectly matches the original disc bytes using `objdiff`.

### Phase 3: Autonomous Orchestration (Upcoming)
- [ ] Draft `scripts/auto_decomp.py` to act as the central AI orchestrator.
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

