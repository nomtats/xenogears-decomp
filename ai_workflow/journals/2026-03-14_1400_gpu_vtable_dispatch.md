# GPU Vtable Dispatch Functions — 5 Matched

**Date:** 2026-03-14  
**Functions:** DrawSync, LoadImage, StoreImage, DrawOTag, DrawPrim  
**Module:** `src/slus_006.64/psyq/libgpu.c`  
**Pattern:** KB pattern_example_015 (PsyQ GPU Vtable Dispatch)

## Summary

Decompiled all 5 GPU vtable dispatch functions identified as high-value targets in the previous session. All functions dispatch through `D_800568C8`, a global pointer to the GPU driver's function pointer table.

## Technique

Declared `D_800568C8` as `extern u_long *D_800568C8;` and accessed table entries via array indexing with function pointer casts: `((ret_type (*)(args))D_800568C8[offset/4])(args)`.

### Vtable offset map discovered:
- `[2]` (0x08): GPU transfer/DMA dispatch function
- `[5]` (0x14): Send primitive data to GPU
- `[6]` (0x18): DrawOTag DMA parameter
- `[7]` (0x1C): StoreImage DMA parameter
- `[8]` (0x20): LoadImage DMA parameter
- `[0xF]` (0x3C): DrawSync/wait-for-GPU function

### Key insight — DrawPrim variable ordering

DrawPrim was the only function that didn't match on the first attempt. The issue: reading the primitive's length byte (`((u_char *)p)[3]`) *after* the first vtable call caused GCC to allocate it in `a1` (argument register) and skip saving `s1`. Moving the read *before* the first call forced GCC to use `s1` (callee-saved) to preserve the value across the jalr, producing the exact register allocation seen in the original assembly.

## Functions Matched

| Function | ASM Lines | Callers | Attempts | Pattern |
|:---------|:----------|:--------|:---------|:--------|
| DrawSync | 30 | 13 | 1 | Debug check + vtable[0xF](mode) |
| DrawOTag | 32 | 4 | 1 | Debug check + vtable[2](vtable[6], ot, 0, 0) |
| LoadImage | 27 | 8 | 1 | func_8004463C + vtable[2](vtable[8], rect, 8, p) |
| StoreImage | 27 | 3 | 1 | func_8004463C + vtable[2](vtable[7], rect, 8, p) |
| DrawPrim | 26 | - | 2 | vtable[0xF](0), read len, vtable[5](p+4, len) |

## What Worked

- The vtable dispatch pattern is highly systematic — once the first function (DrawSync) matched, the remaining 4 followed the same template with minimal variation.
- The `extern u_long *` declaration for the dispatch table was the simplest correct type and produced perfect code generation.
- Functions with `g_GraphDebugLevel >= 2` guards all use the same `sltiu/bnez` pattern.

## What to Do Next

1. **More libgpu vtable functions** — ClearImage, ClearImage2, MoveImage all use the same D_800568C8 dispatch. ClearOTag/ClearOTagR may also use it.
2. **PutDrawEnv/DrawOTagEnv** — More complex (65-75 lines) but also use the vtable with memcpy-like loops.
3. **Remaining libc functions** — func_8003FA78, func_8003FB20, func_8003FB84.
4. **libapi trivial getters** — func_8004076C/func_8004077C.
