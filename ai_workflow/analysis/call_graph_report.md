# Xenogears Decompilation & Call Graph Analysis

## 📈 Global Progress
- **Total Functions:** 1926
- **Decompiled (Matched):** 681
- **Remaining (Unmatched):** 1245
- **Overall Completion:** 35.36%

## 📁 Module Progress
| Module | Matched | Unmatched | Total | Progress |
| :--- | :--- | :--- | :--- | :--- |
| `src/field/effects` | 8 | 2 | 10 | 80.0% |
| `src/field/main/init` | 2 | 1 | 3 | 66.7% |
| `src/field/main/main` | 2 | 12 | 14 | 14.3% |
| `src/field/main/misc` | 100 | 380 | 480 | 20.8% |
| `src/field/main/misc2` | 8 | 36 | 44 | 18.2% |
| `src/field/main/misc3` | 3 | 8 | 11 | 27.3% |
| `src/field/main/misc4` | 29 | 90 | 119 | 24.4% |
| `src/field/main/misc5` | 19 | 59 | 78 | 24.4% |
| `src/field/main/misc6` | 36 | 27 | 63 | 57.1% |
| `src/field/scripts` | 46 | 10 | 56 | 82.1% |
| `src/slus_006.64/main` | 8 | 2 | 10 | 80.0% |
| `src/slus_006.64/psyq/libapi` | 5 | 21 | 26 | 19.2% |
| `src/slus_006.64/psyq/libapi_2` | 0 | 8 | 8 | 0.0% |
| `src/slus_006.64/psyq/libapi_3` | 0 | 1 | 1 | 0.0% |
| `src/slus_006.64/psyq/libc` | 5 | 7 | 12 | 41.7% |
| `src/slus_006.64/psyq/libc2` | 4 | 0 | 4 | 100.0% |
| `src/slus_006.64/psyq/libcard` | 1 | 6 | 7 | 14.3% |
| `src/slus_006.64/psyq/libcd` | 49 | 0 | 49 | 100.0% |
| `src/slus_006.64/psyq/libetc` | 25 | 3 | 28 | 89.3% |
| `src/slus_006.64/psyq/libgpu` | 43 | 23 | 66 | 65.2% |
| `src/slus_006.64/psyq/libgte` | 2 | 92 | 94 | 2.1% |
| `src/slus_006.64/psyq/libsn` | 6 | 2 | 8 | 75.0% |
| `src/slus_006.64/psyq/libspu` | 43 | 0 | 43 | 100.0% |
| `src/slus_006.64/system/animation_scripts` | 4 | 30 | 34 | 11.8% |
| `src/slus_006.64/system/archive` | 9 | 2 | 11 | 81.8% |
| `src/slus_006.64/system/controller` | 11 | 0 | 11 | 100.0% |
| `src/slus_006.64/system/font` | 28 | 0 | 28 | 100.0% |
| `src/slus_006.64/system/graphics` | 0 | 2 | 2 | 0.0% |
| `src/slus_006.64/system/heap_debug` | 3 | 0 | 3 | 100.0% |
| `src/slus_006.64/system/kernel_menu` | 4 | 0 | 4 | 100.0% |
| `src/slus_006.64/system/libarchive` | 21 | 6 | 27 | 77.8% |
| `src/slus_006.64/system/memory` | 40 | 0 | 40 | 100.0% |
| `src/slus_006.64/system/rendering` | 0 | 20 | 20 | 0.0% |
| `src/slus_006.64/system/sound` | 92 | 179 | 271 | 33.9% |
| `src/slus_006.64/system/temp1` | 3 | 72 | 75 | 4.0% |
| `src/slus_006.64/system/temp2` | 0 | 102 | 102 | 0.0% |
| `src/slus_006.64/system/temp3` | 14 | 21 | 35 | 40.0% |
| `src/slus_006.64/system/work_list` | 8 | 21 | 29 | 27.6% |

## 🏆 Top 20 Candidates for Decompilation (Global)
The best un-matched targets across the entire codebase, prioritizing 'True Leaf' status, then caller count, then lowest ASM lines.
| Function | Module | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `DrawSync` | `src/slus_006.64/psyq/libgpu` | 13 | 0 | 30 |
| `func_80072254` | `src/field/main/misc2` | 13 | 0 | 42 |
| `LoadImage` | `src/slus_006.64/psyq/libgpu` | 8 | 0 | 27 |
| `PutDrawEnv` | `src/slus_006.64/psyq/libgpu` | 7 | 0 | 69 |
| `PutDispEnv` | `src/slus_006.64/psyq/libgpu` | 7 | 0 | 335 |
| `func_80085634` | `src/field/main/misc4` | 4 | 0 | 21 |
| `DrawOTag` | `src/slus_006.64/psyq/libgpu` | 4 | 0 | 32 |
| `func_8009E574` | `src/field/main/misc6` | 4 | 0 | 171 |
| `ChangeClearPAD` | `src/slus_006.64/psyq/libapi` | 3 | 0 | 10 |
| `StoreImage` | `src/slus_006.64/psyq/libgpu` | 3 | 0 | 27 |
| `ClearImage` | `src/slus_006.64/psyq/libgpu` | 3 | 0 | 39 |
| `func_80039FF8` | `src/slus_006.64/system/sound` | 3 | 0 | 43 |
| `MoveImage` | `src/slus_006.64/psyq/libgpu` | 3 | 0 | 53 |
| `PCread` | `src/slus_006.64/psyq/libsn` | 3 | 0 | 55 |
| `func_8003B644` | `src/slus_006.64/system/sound` | 3 | 0 | 200 |
| `HookEntryInt` | `src/slus_006.64/psyq/libapi_2` | 2 | 0 | 6 |
| `ChangeClearRCnt` | `src/slus_006.64/psyq/libapi_3` | 2 | 0 | 6 |
| `func_8009E810` | `src/field/main/misc6` | 2 | 0 | 13 |
| `SoundWaitSpuTransfer` | `src/slus_006.64/system/sound` | 2 | 0 | 32 |
| `RotAverage4` | `src/slus_006.64/psyq/libgte` | 2 | 0 | 35 |

## 🎯 Top Targets by Module
Unmatched functions prioritized by 'True Leaf' status, then by mostly heavily referenced.

### `src/field/effects`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldDistortionInitialize` | `distortion.c` | 1 | 0 | 297 |
| `FieldDistortionDraw` | `distortion.c` | 0 | 0 | 535 |

### `src/field/main/init`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldInitializeControllersAndMouse` | `init.c` | 0 | 0 | 33 |

### `src/field/main/main`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80077C60` | `main.c` | 0 | 0 | 12 |
| `FieldInitializeControllers` | `main.c` | 0 | 0 | 16 |
| `func_80077844` | `main.c` | 0 | 0 | 18 |
| `func_800777DC` | `main.c` | 0 | 0 | 19 |
| `func_80077DAC` | `main.c` | 0 | 0 | 28 |
| `func_80077E10` | `main.c` | 0 | 0 | 33 |
| `func_80077D2C` | `main.c` | 0 | 0 | 34 |
| `func_80077C88` | `main.c` | 0 | 0 | 43 |
| `func_80077AB4` | `main.c` | 0 | 0 | 112 |
| `FieldLoadUITextures` | `main.c` | 0 | 0 | 115 |

### `src/field/main/misc`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80092404` | `misc.c` | 0 | 0 | 10 |
| `func_80088D18` | `misc.c` | 0 | 0 | 10 |
| `func_8009A514` | `misc.c` | 0 | 0 | 10 |
| `func_800923E4` | `misc.c` | 0 | 0 | 10 |
| `func_80088CF8` | `misc.c` | 0 | 0 | 10 |
| `func_800961F0` | `misc.c` | 0 | 0 | 11 |
| `func_8009C104` | `misc.c` | 0 | 0 | 12 |
| `func_80087C0C` | `misc.c` | 0 | 0 | 12 |
| `func_800961C8` | `misc.c` | 0 | 0 | 12 |
| `func_8009C0DC` | `misc.c` | 0 | 0 | 12 |

### `src/field/main/misc2`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80072254` | `misc2.c` | 13 | 0 | 42 |
| `func_80076AC0` | `misc2.c` | 1 | 0 | 459 |
| `func_80073734` | `misc2.c` | 0 | 0 | 9 |
| `func_80073988` | `misc2.c` | 0 | 0 | 18 |
| `func_80075B08` | `misc2.c` | 0 | 0 | 18 |
| `func_80076A74` | `misc2.c` | 0 | 0 | 21 |
| `func_800771B0` | `misc2.c` | 0 | 0 | 22 |
| `func_80072398` | `misc2.c` | 0 | 0 | 24 |
| `func_8007234C` | `misc2.c` | 0 | 0 | 24 |
| `func_800722F4` | `misc2.c` | 0 | 0 | 25 |

### `src/field/main/misc3`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80070560` | `misc3.c` | 0 | 0 | 15 |
| `func_80070C84` | `misc3.c` | 0 | 0 | 20 |
| `func_80070508` | `misc3.c` | 0 | 0 | 25 |
| `func_80070488` | `misc3.c` | 0 | 0 | 35 |
| `func_8006FDEC` | `misc3.c` | 0 | 0 | 170 |
| `FieldFree` | `misc3.c` | 0 | 0 | 175 |
| `func_800705DC` | `misc3.c` | 0 | 0 | 435 |
| `FieldLoad` | `misc3.c` | 0 | 0 | 904 |

### `src/field/main/misc4`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80085634` | `misc4.c` | 4 | 0 | 21 |
| `func_8007F6F8` | `misc4.c` | 2 | 0 | 74 |
| `func_8007CD60` | `misc4.c` | 0 | 0 | 10 |
| `func_8007CD3C` | `misc4.c` | 0 | 0 | 11 |
| `func_80081F5C` | `misc4.c` | 0 | 0 | 11 |
| `func_800831D0` | `misc4.c` | 0 | 0 | 11 |
| `func_8007C670` | `misc4.c` | 0 | 0 | 13 |
| `func_8007B694` | `misc4.c` | 0 | 0 | 14 |
| `func_80086D4C` | `misc4.c` | 0 | 0 | 18 |
| `func_800864B4` | `misc4.c` | 0 | 0 | 18 |

### `src/field/main/misc5`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldZoomFadeEffectUpdate` | `misc5.c` | 1 | 0 | 145 |
| `FieldZoomFadeEffectInitialize` | `misc5.c` | 1 | 0 | 190 |
| `func_800A55B8` | `misc5.c` | 0 | 0 | 6 |
| `func_800A7064` | `misc5.c` | 0 | 0 | 12 |
| `func_800A55C8` | `misc5.c` | 0 | 0 | 16 |
| `func_800A987C` | `misc5.c` | 0 | 0 | 16 |
| `func_800A9274` | `misc5.c` | 0 | 0 | 17 |
| `func_800A98B4` | `misc5.c` | 0 | 0 | 17 |
| `func_800A9B1C` | `misc5.c` | 0 | 0 | 18 |
| `func_800A9460` | `misc5.c` | 0 | 0 | 20 |

### `src/field/main/misc6`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8009E574` | `misc6.c` | 4 | 0 | 171 |
| `func_8009E810` | `misc6.c` | 2 | 0 | 13 |
| `func_800A0C94` | `misc6.c` | 1 | 0 | 44 |
| `func_8009F5F4` | `misc6.c` | 1 | 0 | 278 |
| `func_800A0EB0` | `misc6.c` | 0 | 0 | 16 |
| `func_800A0DC0` | `misc6.c` | 0 | 0 | 17 |
| `func_800A0C4C` | `misc6.c` | 0 | 0 | 20 |
| `func_800A0DFC` | `misc6.c` | 0 | 0 | 24 |
| `func_800A0E54` | `misc6.c` | 0 | 0 | 27 |
| `func_8009FCAC` | `misc6.c` | 0 | 0 | 28 |

### `src/field/scripts`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldScriptVMRun` | `virtual_machine.c` | 1 | 0 | 98 |
| `func_800A3C8C` | `virtual_machine.c` | 1 | 0 | 191 |
| `func_800A2488` | `virtual_machine.c` | 0 | 0 | 17 |
| `func_800A30FC` | `virtual_machine.c` | 0 | 0 | 62 |
| `func_800A24C4` | `virtual_machine.c` | 0 | 0 | 158 |
| `func_800A2030` | `virtual_machine.c` | 0 | 0 | 174 |
| `func_800A31E8` | `virtual_machine.c` | 0 | 0 | 174 |
| `func_800A28D4` | `virtual_machine.c` | 0 | 0 | 461 |
| `func_800A3F4C` | `virtual_machine.c` | 0 | 0 | 541 |
| `func_800A3474` | `virtual_machine.c` | 0 | 0 | 545 |

### `src/slus_006.64/main`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `GameSoftReset` | `main_loop.c` | 1 | 0 | 32 |
| `func_80019578` | `main.c` | 0 | 0 | 256 |

### `src/slus_006.64/psyq/libapi`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `ChangeClearPAD` | `libapi.c` | 3 | 0 | 10 |
| `WaitEvent` | `WaitEvent.c` | 1 | 0 | 6 |
| `func_8004076C` | `libapi.c` | 0 | 0 | 6 |
| `SysEnqIntRP` | `libapi.c` | 0 | 0 | 6 |
| `InitPAD2` | `libapi.c` | 0 | 0 | 6 |
| `StopPAD2` | `libapi.c` | 0 | 0 | 6 |
| `SysDeqIntRP` | `libapi.c` | 0 | 0 | 6 |
| `StartPAD2` | `libapi.c` | 0 | 0 | 6 |
| `func_8004077C` | `libapi.c` | 0 | 0 | 6 |
| `func_80040ABC` | `libapi.c` | 0 | 0 | 6 |

### `src/slus_006.64/psyq/libapi_2`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `HookEntryInt` | `libapi_2.c` | 2 | 0 | 6 |
| `ResetEntryInt` | `libapi_2.c` | 1 | 0 | 6 |
| `func_8004BED8` | `libapi_2.c` | 1 | 0 | 8 |
| `setjmp` | `libapi_2.c` | 1 | 0 | 17 |
| `func_8004BED0` | `libapi_2.c` | 0 | 0 | 4 |
| `ReturnFromException` | `libapi_2.c` | 0 | 0 | 6 |
| `func_8004BEC0` | `libapi_2.c` | 0 | 0 | 6 |
| `longjmp` | `libapi_2.c` | 0 | 0 | 15 |

### `src/slus_006.64/psyq/libapi_3`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `ChangeClearRCnt` | `libapi_3.c` | 2 | 0 | 6 |

### `src/slus_006.64/psyq/libc`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `Sprintf` | `libc.c` | 2 | 0 | 604 |
| `memset` | `libc.c` | 0 | 0 | 16 |
| `bzero` | `libc.c` | 0 | 0 | 16 |
| `func_8003FB84` | `libc.c` | 0 | 0 | 22 |
| `memchr` | `libc.c` | 0 | 0 | 26 |
| `func_8003FB20` | `libc.c` | 0 | 0 | 33 |
| `func_8003FA78` | `libc.c` | 0 | 0 | 49 |

### `src/slus_006.64/psyq/libcard`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `StartCARD2` | `libcard.c` | 1 | 0 | 6 |
| `_card_info` | `libcard.c` | 0 | 0 | 6 |
| `StopCARD2` | `libcard.c` | 0 | 0 | 6 |
| `InitCARD2` | `libcard.c` | 0 | 0 | 6 |
| `StopCARD` | `libcard.c` | 0 | 0 | 14 |
| `InitCARD` | `libcard.c` | 0 | 0 | 23 |

### `src/slus_006.64/psyq/libetc`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `trapIntr` | `intr.c` | 1 | 0 | 132 |
| `setIntrDMA` | `intr_dma.c` | 0 | 0 | 46 |
| `trapIntrDMA` | `intr_dma.c` | 0 | 0 | 107 |

### `src/slus_006.64/psyq/libgpu`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `DrawSync` | `libgpu.c` | 13 | 0 | 30 |
| `LoadImage` | `libgpu.c` | 8 | 0 | 27 |
| `PutDrawEnv` | `libgpu.c` | 7 | 0 | 69 |
| `PutDispEnv` | `libgpu.c` | 7 | 0 | 335 |
| `DrawOTag` | `libgpu.c` | 4 | 0 | 32 |
| `StoreImage` | `libgpu.c` | 3 | 0 | 27 |
| `ClearImage` | `libgpu.c` | 3 | 0 | 39 |
| `MoveImage` | `libgpu.c` | 3 | 0 | 53 |
| `ClearOTagR` | `libgpu.c` | 2 | 0 | 41 |
| `DrawPrim` | `libgpu.c` | 1 | 0 | 26 |

### `src/slus_006.64/psyq/libgte`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `RotAverage4` | `libgte.c` | 2 | 0 | 35 |
| `PushMatrix` | `libgte.c` | 2 | 0 | 44 |
| `PopMatrix` | `libgte.c` | 2 | 0 | 44 |
| `SetGeomScreen` | `libgte.c` | 1 | 0 | 7 |
| `SetGeomOffset` | `libgte.c` | 1 | 0 | 11 |
| `InitGeom` | `libgte.c` | 1 | 0 | 37 |
| `func_8004A0A4` | `libgte.c` | 0 | 0 | 6 |
| `SetVertex0` | `libgte.c` | 0 | 0 | 6 |
| `func_8004A098` | `libgte.c` | 0 | 0 | 6 |
| `func_8004A0B0` | `libgte.c` | 0 | 0 | 6 |

### `src/slus_006.64/psyq/libsn`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `PCread` | `libsn.c` | 3 | 0 | 55 |
| `PCwrite` | `libsn.c` | 2 | 0 | 55 |

### `src/slus_006.64/system/animation_scripts`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80021BCC` | `animation_scripts.c` | 1 | 0 | 11 |
| `func_80022224` | `animation_scripts.c` | 1 | 0 | 41 |
| `func_80022A00` | `animation_scripts.c` | 0 | 0 | 5 |
| `func_80021B14` | `animation_scripts.c` | 0 | 0 | 6 |
| `func_80021B04` | `animation_scripts.c` | 0 | 0 | 6 |
| `func_80021D3C` | `animation_scripts.c` | 0 | 0 | 7 |
| `AnimScriptStackPopU8` | `animation_scripts.c` | 0 | 0 | 9 |
| `func_80021FE0` | `animation_scripts.c` | 0 | 0 | 10 |
| `func_80021C00` | `animation_scripts.c` | 0 | 0 | 10 |
| `func_80021FC0` | `animation_scripts.c` | 0 | 0 | 10 |

### `src/slus_006.64/system/archive`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80029AFC` | `archive.c` | 2 | 0 | 261 |
| `func_80029EB0` | `archive.c` | 0 | 0 | 248 |

### `src/slus_006.64/system/graphics`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `GfxLoadImageAccelerated` | `graphics.c` | 0 | 0 | 27 |
| `GfxLoadClutsAccelerated` | `graphics.c` | 0 | 0 | 51 |

### `src/slus_006.64/system/libarchive`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8002954C` | `libarchive.c` | 1 | 0 | 39 |
| `func_8002945C` | `libarchive.c` | 0 | 0 | 26 |
| `func_80028E60` | `libarchive.c` | 0 | 0 | 33 |
| `func_800294B4` | `libarchive.c` | 0 | 0 | 45 |
| `func_80028B14` | `libarchive.c` | 0 | 0 | 231 |
| `func_80028F30` | `libarchive.c` | 0 | 0 | 352 |

### `src/slus_006.64/system/rendering`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8001EE68` | `rendering.c` | 1 | 0 | 5 |
| `func_8001EE74` | `rendering.c` | 0 | 0 | 7 |
| `func_8001D4E8` | `rendering.c` | 0 | 0 | 24 |
| `func_8001E298` | `rendering.c` | 0 | 0 | 27 |
| `func_8001E2F8` | `rendering.c` | 0 | 0 | 31 |
| `func_8001E368` | `rendering.c` | 0 | 0 | 31 |
| `func_8001FB30` | `rendering.c` | 0 | 0 | 31 |
| `GraphicsDrawPauseLetters` | `rendering.c` | 0 | 0 | 33 |
| `func_8001F530` | `rendering.c` | 0 | 0 | 38 |
| `func_8001F6B0` | `rendering.c` | 0 | 0 | 45 |

### `src/slus_006.64/system/sound`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80039FF8` | `sound.c` | 3 | 0 | 43 |
| `func_8003B644` | `sound.c` | 3 | 0 | 200 |
| `SoundWaitSpuTransfer` | `sound.c` | 2 | 0 | 32 |
| `SoundHeapFree` | `sound.c` | 2 | 0 | 39 |
| `SoundSpuMemoryAllocateBlockAtAddress` | `sound.c` | 2 | 0 | 83 |
| `func_8003B930` | `sound.c` | 1 | 0 | 23 |
| `func_8003A89C` | `sound.c` | 1 | 0 | 48 |
| `SoundSpuMemoryAllocateBlock` | `sound.c` | 1 | 0 | 71 |
| `func_80039024` | `sound.c` | 1 | 0 | 80 |
| `func_8003A65C` | `sound.c` | 1 | 0 | 126 |

### `src/slus_006.64/system/temp1`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_800230A8` | `temp1.c` | 1 | 0 | 34 |
| `func_8002804C` | `temp1.c` | 1 | 0 | 126 |
| `GfxSetCurrentOT` | `temp1.c` | 0 | 0 | 6 |
| `func_8002393C` | `temp1.c` | 0 | 0 | 7 |
| `func_800231F8` | `temp1.c` | 0 | 0 | 8 |
| `func_800231E0` | `temp1.c` | 0 | 0 | 8 |
| `func_80027D40` | `temp1.c` | 0 | 0 | 12 |
| `func_80022E8C` | `temp1.c` | 0 | 0 | 13 |
| `func_80023440` | `temp1.c` | 0 | 0 | 13 |
| `GfxFreeWorkBuffers` | `temp1.c` | 0 | 0 | 13 |

### `src/slus_006.64/system/temp2`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8002DFF0` | `temp2.c` | 1 | 0 | 10 |
| `func_800317E0` | `temp2.c` | 1 | 0 | 11 |
| `func_80031804` | `temp2.c` | 1 | 0 | 11 |
| `func_8002C3D8` | `temp2.c` | 0 | 0 | 6 |
| `func_8002DFE0` | `temp2.c` | 0 | 0 | 6 |
| `func_8002BA40` | `temp2.c` | 0 | 0 | 8 |
| `func_8002CCAC` | `temp2.c` | 0 | 0 | 9 |
| `func_8002CC54` | `temp2.c` | 0 | 0 | 10 |
| `func_8002C6E0` | `temp2.c` | 0 | 0 | 10 |
| `func_800315A0` | `temp2.c` | 0 | 0 | 11 |

### `src/slus_006.64/system/temp3`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `GamePartyGearsInitializeSkins` | `temp3.c` | 1 | 0 | 106 |
| `func_8001BB0C` | `temp3.c` | 0 | 0 | 19 |
| `func_8001BEEC` | `temp3.c` | 0 | 0 | 21 |
| `func_8001B66C` | `temp3.c` | 0 | 0 | 23 |
| `func_8001BB50` | `temp3.c` | 0 | 0 | 25 |
| `func_8001A684` | `temp3.c` | 0 | 0 | 31 |
| `func_8001B5E8` | `temp3.c` | 0 | 0 | 38 |
| `func_8001BD40` | `temp3.c` | 0 | 0 | 45 |
| `func_8001A5CC` | `temp3.c` | 0 | 0 | 50 |
| `func_8001BE14` | `temp3.c` | 0 | 0 | 56 |

### `src/slus_006.64/system/work_list`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `WorkListRemoveTask` | `work_list.c` | 2 | 0 | 49 |
| `TimerWorkListRemoveTask` | `work_list.c` | 2 | 0 | 52 |
| `func_8001D2A4` | `work_list.c` | 0 | 0 | 5 |
| `func_8001D298` | `work_list.c` | 0 | 0 | 5 |
| `WorkListsReset` | `work_list.c` | 0 | 0 | 10 |
| `func_8001D164` | `work_list.c` | 0 | 0 | 18 |
| `WorkListAllocateTask` | `work_list.c` | 0 | 0 | 24 |
| `TimerWorkListAllocateTask` | `work_list.c` | 0 | 0 | 25 |
| `func_8001D10C` | `work_list.c` | 0 | 0 | 27 |
| `WorkListUpdate` | `work_list.c` | 0 | 0 | 29 |

## 👑 Most Highly Referenced Functions Overall
Functions called by the most other functions throughout the entire codebase.
| Function | Callers | Module | Matched? |
| :--- | :--- | :--- | :--- |
| `FieldScriptVMGetInstructionArgument` | 46 | `src/field/main/misc5.c` | ✅ |
| `FieldScriptVMGetArgument` | 37 | `src/field/main/misc5.c` | ✅ |
| `FieldScriptMemoryWriteU16` | 35 | `src/field/scripts/virtual_machine.c` | ✅ |
| `HeapFree` | 29 | `src/slus_006.64/system/memory.c` | ✅ |
| `FieldScriptVMGetVariableValue` | 27 | `src/field/scripts/virtual_machine.c` | ✅ |
| `v0` | 22 | `Unknown` | ✅ |
| `HeapAlloc` | 20 | `src/slus_006.64/system/memory.c` | ✅ |
| `Vsync` | 19 | `src/slus_006.64/psyq/libetc/vsync.c` | ✅ |
| `func_8009CFBC` | 15 | `src/field/scripts/variable_handlers.c` | ✅ |
| `CdSyncCallback` | 13 | `src/slus_006.64/psyq/libcd/sys.c` | ✅ |
| `DrawSync` | 13 | `src/slus_006.64/psyq/libgpu.c` | ❌ |
| `func_80072254` | 13 | `src/field/main/misc2.c` | ❌ |
| `FieldScriptVMGetActorIndex` | 12 | `src/field/main/misc.c` | ✅ |
| `CdReadyCallback` | 10 | `src/slus_006.64/psyq/libcd/sys.c` | ✅ |
| `printf` | 10 | `Unknown` | ✅ |
| `SoundHandleError` | 10 | `src/slus_006.64/system/sound.c` | ✅ |
| `FieldScriptVMGetInstructionArgumentS16` | 10 | `src/field/main/misc5.c` | ✅ |
| `ExitCriticalSection` | 9 | `Unknown` | ✅ |
| `puts` | 9 | `src/slus_006.64/psyq/libc2/puts.c` | ✅ |
| `CdControlF` | 9 | `src/slus_006.64/psyq/libcd/sys.c` | ✅ |