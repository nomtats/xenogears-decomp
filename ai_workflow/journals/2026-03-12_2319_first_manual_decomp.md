# Journal Entry: 2026-03-12 23:19 - First Successful Manual Decompilation

## Objective
To manually test the entire decompilation pipeline (from finding an unmatched assembly file to verifying a compiled hash output against the PS1 disc) before writing the autonomous orchestrator script. 

## Actions Taken
1.  **Target Selection**: Executed the `tools/scripts/find_target.py` analysis script logic. I singled out `func_80021FB8.s` inside `asm/slus_006.64/nonmatchings/system/animation_scripts/` as the easiest candidate. It was a 4-line leaf function consisting solely of an `sb` (store byte) instruction and a return (`jr $ra`).
2.  **Contextual Analysis**: 
    - Analyzed the target assembly: `sb $a1, 0xB0($a0)`
    - Used `grep_search` to locate the `INCLUDE_ASM` macro inside `src/slus_006.64/system/animation_scripts.c`.
    - Referenced the neighboring sibling functions in C and determined that `$a0` was universally passed as a pointer to the `SpriteData` struct (defined in `include/field/actor.h`).
3.  **C Code Generation**: 
    - Exchanged the `INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021FB8);` macro for the actual C implementation.
    - Since `$a1` was stored into an offset of `0xB0` as a raw byte, I cast the `SpriteData` pointer to a `u8*` and assigned the value: 
      ```c
      void func_80021FB8(SpriteData* pSpriteData, u8 val) {
          ((u8*)pSpriteData)[0xB0] = val;
      }
      ```
4.  **Verification and Diffing**:
    - Purged stale `.o` binaries using `make clean-build` inside the Docker `xenogears_decomp_env` daemon.
    - Successfully compiled `animation_scripts.c`.
    - Executed `make check` and received a perfectly matching `SLUS_006.64: OK` hash comparison.
    - Executed `make report`. Our matched functions metric officially increased from **672** to **673**.

## Meta-Learnings & DKB Updates
- The build pipeline occasionally retains stale intermediary object files that can cause undefined reference link errors when replacing `INCLUDE_ASM` macros. `make clean-build` is the safest way to guarantee a fresh verification.
- Casting struct pointers to byte arrays (`(u8*)`) is an extremely highly effective method for achieving 1-to-1 matching for simple `sb` load/store offset operations in PsyQ.
- I have recorded these strategies directly into `ai_workflow/knowledge_base.json`.

## Next Logical Steps
Phase 3 is entirely checked off. The underlying compilation architecture is stable and fully understood. We are ready to begin Phase 4 by writing `auto_decomp.py` to automate exactly what I just performed manually.
