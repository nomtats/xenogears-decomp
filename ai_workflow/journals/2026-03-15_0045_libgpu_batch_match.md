# Continuing libgpu Phase 4 Decompilation (SetDrawMove, PutDrawEnv, etc.)

## Goal
The goal here was to decompile the remaining "easy" libgpu functions according to the user's prompt. We successfully matched `SetDrawMove`, `PutDrawEnv`, `DrawOTagEnv`, `SetGraphReverse`, and `SetGraphDebug`.

## Process
1. **PutDrawEnv and DrawOTagEnv**: 
    - These functions were very similar. They called the internal wrapper `func_8004574C(dr_env, env)`, setup the linked lists, fired DMA using `g_GpuPkg->dmaTransfer`, and lastly saved the env into `g_GpuDrawEnv`.
    - However, GCC decided to access `g_GpuDrawEnv` using a cached register to `g_GraphDebugLevel`! Because the two `extern` globals are 14-bytes apart in RAM due to linking layout, GCC did an offset load (`&g_GraphDebugLevel + 14`).
    - We replicated this exactly by declaring `char *debugLvl = &g_GraphDebugLevel;` and caching it in C, getting a 100% byte match.

2. **SetDrawMove**:
    - This required an exact structural match for setting up lengths with branch polarization.
    - We used the goto state-machine technique to force the compiler to assign `len = 5` up front, and branch out of assigning `len = 0` only when condition is met. Perfect match.

3. **SetGraphReverse and SetGraphDebug**:
    - Needed manual pointer caching to emulate how GCC hoisted addresses into callee-saved registers before calls.
    - Used standard ternary operator translation: `(mode ? 0x08000080 : 0x08000000)` which successfully generated the necessary `lui` and `ori` sequence.

## Outcome
- All 5 functions successfully hit a perfect 100% match.
- Left remaining libgpu unmatched to: 8 functions.

## Next Steps
- Continue with remaining libgpu functions: `PutDispEnv`, `ResetGraph`, `SetDispMask` (note: wait until `func_80047178` is decompiled for this one or declare it as an extern explicitly), and `func_8004xxxx`.
