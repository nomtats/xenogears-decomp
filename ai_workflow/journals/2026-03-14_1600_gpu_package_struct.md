# GPU Package Struct & Symbol Rename

**Date**: 2026-03-14
**Category**: Readability Improvement / Refactoring
**Result**: Byte-perfect match confirmed after full rename

## Summary

Defined a `GpuPackage` struct in `include/psyq/libgpu.h` to replace the raw `u_long*` array access pattern for the GPU driver vtable. Renamed the global symbol `D_800568C8` to `g_GpuPkg` across the entire repository.

## Vtable Offset Map

| Offset | Index | Type | Struct Member | Used by |
|:-------|:------|:-----|:-------------|:--------|
| 0x00 | 0 | u_long | unk_00 | (unknown) |
| 0x04 | 1 | u_long | unk_04 | (unknown) |
| 0x08 | 2 | func ptr | transfer | LoadImage, StoreImage, DrawOTag, ClearImage, ClearImage2, MoveImage, PutDrawEnv, DrawOTagEnv |
| 0x0C | 3 | u_long | clearParam | ClearImage, ClearImage2 |
| 0x10 | 4 | func ptr | command | SetDispMask, PutDispEnv |
| 0x14 | 5 | func ptr | sendData | DrawPrim |
| 0x18 | 6 | u_long | otagParam | DrawOTag, MoveImage, PutDrawEnv, DrawOTagEnv |
| 0x1C | 7 | u_long | storeParam | StoreImage |
| 0x20 | 8 | u_long | loadParam | LoadImage |
| 0x24 | 9 | u_long | unk_24 | (unknown) |
| 0x28 | 10 | u_long | unk_28 | (unknown) |
| 0x2C | 11 | func ptr | clearOTagR | ClearOTagR |
| 0x30 | 12 | u_long | unk_30 | (unknown) |
| 0x34 | 13 | func ptr | reset | ResetGraph, func_8004440C |
| 0x38 | 14 | u_long | unk_38 | (unknown) |
| 0x3C | 15 | func ptr | drawSync | DrawSync, DrawPrim |

## Key Design Decision: K&R Function Pointers

Function pointer members use K&R-style empty parentheses `()` (unspecified arguments) instead of typed prototypes. This allows callers to pass whatever arguments they need without explicit casts:

```c
// Before (noisy casts):
return ((int (*)(int))D_800568C8[0xF])(mode);
return ((int (*)(u_long, RECT *, int, u_long *))D_800568C8[2])(D_800568C8[8], rect, 8, p);

// After (clean struct access):
return g_GpuPkg->drawSync(mode);
return g_GpuPkg->transfer(g_GpuPkg->loadParam, rect, 8, p);
```

This works because GCC 2.7.2 treats `()` as "accepts any arguments" (pre-C99 behavior), and `sizeof(function_ptr) == sizeof(u_long) == 4` on MIPS32, so the struct layout is identical to the original `u_long` array.

## Files Changed

- `include/psyq/libgpu.h` — Added `GpuPackage` typedef struct
- `config/symbol_addrs.slus_006.64.txt` — Added `g_GpuPkg = 0x800568C8;`
- `src/slus_006.64/psyq/libgpu.c` — Changed extern to `GpuPackage *g_GpuPkg`, updated all 5 decompiled function bodies
- 19 assembly files — Global rename `D_800568C8` → `g_GpuPkg`

## Verification

`make check` passed: both `SLUS_006.64: OK` and `FIELD.BIN: OK`.
