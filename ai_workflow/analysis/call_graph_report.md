# Xenogears Decompilation & Call Graph Analysis

## 📈 Global Progress
- **Total Functions:** 1927
- **Decompiled (Matched):** 680
- **Remaining (Unmatched):** 1247
- **Overall Completion:** 35.29%

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
| `src/slus_006.64/psyq/libgpu` | 42 | 24 | 66 | 63.6% |
| `src/slus_006.64/psyq/libgte` | 2 | 92 | 94 | 2.1% |
| `src/slus_006.64/psyq/libsn` | 6 | 2 | 8 | 75.0% |
| `src/slus_006.64/psyq/libspu` | 42 | 1 | 43 | 97.7% |
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
| `src/slus_006.64/system/temp2` | 1 | 102 | 103 | 1.0% |
| `src/slus_006.64/system/temp3` | 14 | 21 | 35 | 40.0% |
| `src/slus_006.64/system/work_list` | 8 | 21 | 29 | 27.6% |

## 🏆 Top 20 Candidates for Decompilation (Global)
The best un-matched targets across the entire codebase, prioritizing 'True Leaf' status, then caller count, then lowest ASM lines.
| Function | Module | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `NormalClip` | `src/slus_006.64/psyq/libgte` | 10 | 0 | 15 |
| `func_8008CF3C` | `src/field/main/misc` | 10 | 0 | 30 |
| `CompMatrix` | `src/slus_006.64/psyq/libgte` | 10 | 0 | 91 |
| `SetGeomScreen` | `src/slus_006.64/psyq/libgte` | 8 | 0 | 7 |
| `func_8009A514` | `src/field/main/misc` | 8 | 0 | 10 |
| `SoundWaitSpuTransfer` | `src/slus_006.64/system/sound` | 8 | 0 | 32 |
| `RotAverage4` | `src/slus_006.64/psyq/libgte` | 8 | 0 | 35 |
| `ScaleMatrix` | `src/slus_006.64/psyq/libgte` | 8 | 0 | 78 |
| `ChangeClearPAD` | `src/slus_006.64/psyq/libapi` | 7 | 0 | 10 |
| `SetGeomOffset` | `src/slus_006.64/psyq/libgte` | 7 | 0 | 11 |
| `RotTransPers` | `src/slus_006.64/psyq/libgte` | 7 | 0 | 15 |
| `func_800234AC` | `src/slus_006.64/system/temp1` | 7 | 0 | 38 |
| `func_80048C4C` | `src/slus_006.64/psyq/libgte` | 7 | 0 | 42 |
| `func_800A0C94` | `src/field/main/misc6` | 7 | 0 | 44 |
| `func_8004A414` | `src/slus_006.64/psyq/libgte` | 6 | 0 | 13 |
| `func_8003E5BC` | `src/slus_006.64/system/sound` | 6 | 0 | 51 |
| `func_80080968` | `src/field/main/misc4` | 5 | 0 | 29 |
| `MulMatrix2` | `src/slus_006.64/psyq/libgte` | 5 | 0 | 71 |
| `func_8004A19C` | `src/slus_006.64/psyq/libgte` | 4 | 0 | 9 |
| `func_8004A260` | `src/slus_006.64/psyq/libgte` | 4 | 0 | 10 |

## 🎯 Top Targets by Module
Unmatched functions prioritized by 'True Leaf' status, then by mostly heavily referenced.

### `src/field/effects`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldDistortionInitialize` | `distortion.c` | 2 | 7 | 297 |
| `FieldDistortionDraw` | `distortion.c` | 1 | 1 | 535 |

### `src/field/main/init`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldInitializeControllersAndMouse` | `init.c` | 1 | 4 | 33 |

### `src/field/main/main`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80077844` | `main.c` | 1 | 0 | 18 |
| `func_80077E10` | `main.c` | 1 | 0 | 33 |
| `func_80077DAC` | `main.c` | 5 | 5 | 28 |
| `func_80077D2C` | `main.c` | 2 | 2 | 34 |
| `func_80077C88` | `main.c` | 2 | 3 | 43 |
| `func_80077AB4` | `main.c` | 2 | 7 | 112 |
| `func_80077884` | `main.c` | 2 | 6 | 149 |
| `func_80077C60` | `main.c` | 1 | 2 | 12 |
| `FieldInitializeControllers` | `main.c` | 1 | 3 | 16 |
| `func_800777DC` | `main.c` | 1 | 2 | 19 |

### `src/field/main/misc`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8008CF3C` | `misc.c` | 10 | 0 | 30 |
| `func_8009A514` | `misc.c` | 8 | 0 | 10 |
| `func_80095124` | `misc.c` | 4 | 0 | 46 |
| `func_800950A0` | `misc.c` | 3 | 0 | 42 |
| `func_8009501C` | `misc.c` | 3 | 0 | 42 |
| `func_80092424` | `misc.c` | 3 | 0 | 52 |
| `func_800924D4` | `misc.c` | 3 | 0 | 61 |
| `func_8008D2E0` | `misc.c` | 2 | 0 | 13 |
| `func_8009744C` | `misc.c` | 2 | 0 | 22 |
| `func_8008A790` | `misc.c` | 2 | 0 | 25 |

### `src/field/main/misc2`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80073930` | `misc2.c` | 2 | 0 | 26 |
| `func_8007234C` | `misc2.c` | 1 | 0 | 24 |
| `func_80072398` | `misc2.c` | 1 | 0 | 24 |
| `func_8007469C` | `misc2.c` | 1 | 0 | 31 |
| `func_80073734` | `misc2.c` | 0 | 0 | 9 |
| `func_80076A74` | `misc2.c` | 0 | 0 | 21 |
| `func_800771B0` | `misc2.c` | 0 | 0 | 22 |
| `func_80072254` | `misc2.c` | 15 | 2 | 42 |
| `func_80076AC0` | `misc2.c` | 10 | 12 | 459 |
| `FieldPollControllers` | `misc2.c` | 5 | 3 | 128 |

### `src/field/main/misc3`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80070C84` | `misc3.c` | 2 | 0 | 20 |
| `func_80070560` | `misc3.c` | 0 | 0 | 15 |
| `func_80070488` | `misc3.c` | 3 | 2 | 35 |
| `func_80070508` | `misc3.c` | 2 | 4 | 25 |
| `FieldFree` | `misc3.c` | 2 | 18 | 175 |
| `func_8006FDEC` | `misc3.c` | 1 | 8 | 170 |
| `func_800705DC` | `misc3.c` | 1 | 11 | 435 |
| `FieldLoad` | `misc3.c` | 1 | 35 | 904 |

### `src/field/main/misc4`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80080968` | `misc4.c` | 5 | 0 | 29 |
| `func_80081C54` | `misc4.c` | 4 | 0 | 198 |
| `func_8007CD60` | `misc4.c` | 2 | 0 | 10 |
| `func_8007CD3C` | `misc4.c` | 2 | 0 | 11 |
| `func_80086078` | `misc4.c` | 2 | 0 | 33 |
| `func_800798BC` | `misc4.c` | 2 | 0 | 46 |
| `func_8008492C` | `misc4.c` | 2 | 0 | 75 |
| `func_80081F5C` | `misc4.c` | 1 | 0 | 11 |
| `func_800831D0` | `misc4.c` | 1 | 0 | 11 |
| `func_800864B4` | `misc4.c` | 1 | 0 | 18 |

### `src/field/main/misc5`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_800A94A4` | `misc5.c` | 4 | 0 | 125 |
| `func_800A98B4` | `misc5.c` | 1 | 0 | 17 |
| `func_800A9274` | `misc5.c` | 1 | 0 | 17 |
| `func_800A9B1C` | `misc5.c` | 1 | 0 | 18 |
| `func_800AB328` | `misc5.c` | 1 | 0 | 25 |
| `func_800A9374` | `misc5.c` | 1 | 0 | 27 |
| `func_800AA9DC` | `misc5.c` | 1 | 0 | 42 |
| `func_800A93CC` | `misc5.c` | 1 | 0 | 43 |
| `func_800AC03C` | `misc5.c` | 1 | 0 | 52 |
| `func_800AB748` | `misc5.c` | 1 | 0 | 58 |

### `src/field/main/misc6`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_800A0C94` | `misc6.c` | 7 | 0 | 44 |
| `func_800A0C4C` | `misc6.c` | 3 | 0 | 20 |
| `func_8009E810` | `misc6.c` | 2 | 0 | 13 |
| `func_800A0EB0` | `misc6.c` | 0 | 0 | 16 |
| `func_800A0E54` | `misc6.c` | 0 | 0 | 27 |
| `func_8009E574` | `misc6.c` | 7 | 2 | 171 |
| `func_8009FA54` | `misc6.c` | 1 | 3 | 86 |
| `func_8009FEE4` | `misc6.c` | 1 | 1 | 165 |
| `func_8009F5F4` | `misc6.c` | 1 | 2 | 278 |
| `func_800A0DC0` | `misc6.c` | 0 | 1 | 17 |

### `src/field/scripts`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `FieldScriptVMRun` | `virtual_machine.c` | 4 | 2 | 98 |
| `func_800A31E8` | `virtual_machine.c` | 4 | 4 | 174 |
| `func_800A30FC` | `virtual_machine.c` | 3 | 4 | 62 |
| `func_800A2030` | `virtual_machine.c` | 2 | 2 | 174 |
| `func_800A2488` | `virtual_machine.c` | 1 | 2 | 17 |
| `func_800A24C4` | `virtual_machine.c` | 1 | 3 | 158 |
| `func_800A3C8C` | `virtual_machine.c` | 1 | 1 | 191 |
| `func_800A28D4` | `virtual_machine.c` | 1 | 7 | 461 |
| `func_800A3F4C` | `virtual_machine.c` | 1 | 2 | 541 |
| `func_800A3474` | `virtual_machine.c` | 1 | 1 | 545 |

### `src/slus_006.64/main`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `GameSoftReset` | `main_loop.c` | 2 | 12 | 32 |
| `func_80019578` | `main.c` | 0 | 43 | 256 |

### `src/slus_006.64/psyq/libapi`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `ChangeClearPAD` | `libapi.c` | 7 | 0 | 10 |
| `SysDeqIntRP` | `libapi.c` | 2 | 0 | 6 |
| `InitPAD2` | `libapi.c` | 1 | 0 | 6 |
| `StartPAD2` | `libapi.c` | 1 | 0 | 6 |
| `StopPAD2` | `libapi.c` | 1 | 0 | 6 |
| `func_80040ABC` | `libapi.c` | 1 | 0 | 6 |
| `SysEnqIntRP` | `libapi.c` | 1 | 0 | 6 |
| `WaitEvent` | `WaitEvent.c` | 1 | 0 | 6 |
| `EnablePAD` | `libapi.c` | 1 | 0 | 7 |
| `func_80040B00` | `libapi.c` | 1 | 0 | 7 |

### `src/slus_006.64/psyq/libapi_2`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `HookEntryInt` | `libapi_2.c` | 2 | 0 | 6 |
| `ResetEntryInt` | `libapi_2.c` | 1 | 0 | 6 |
| `ReturnFromException` | `libapi_2.c` | 1 | 0 | 6 |
| `func_8004BED8` | `libapi_2.c` | 1 | 0 | 8 |
| `setjmp` | `libapi_2.c` | 1 | 0 | 17 |
| `func_8004BED0` | `libapi_2.c` | 0 | 0 | 4 |
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
| `bzero` | `libc.c` | 1 | 0 | 16 |
| `memset` | `libc.c` | 0 | 0 | 16 |
| `func_8003FB84` | `libc.c` | 0 | 0 | 22 |
| `memchr` | `libc.c` | 0 | 0 | 26 |
| `func_8003FB20` | `libc.c` | 0 | 0 | 33 |
| `func_8003FA78` | `libc.c` | 0 | 1 | 49 |

### `src/slus_006.64/psyq/libcard`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `InitCARD2` | `libcard.c` | 1 | 0 | 6 |
| `StartCARD2` | `libcard.c` | 1 | 0 | 6 |
| `StopCARD2` | `libcard.c` | 1 | 0 | 6 |
| `_card_info` | `libcard.c` | 0 | 0 | 6 |
| `InitCARD` | `libcard.c` | 1 | 6 | 23 |
| `StopCARD` | `libcard.c` | 0 | 3 | 14 |

### `src/slus_006.64/psyq/libetc`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `setIntrDMA` | `intr_dma.c` | 0 | 0 | 46 |
| `trapIntr` | `intr.c` | 1 | 3 | 132 |
| `trapIntrDMA` | `intr_dma.c` | 0 | 2 | 107 |

### `src/slus_006.64/psyq/libgpu`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `SetDrawMove` | `libgpu.c` | 1 | 0 | 28 |
| `func_800444B8` | `libgpu.c` | 0 | 0 | 6 |
| `D_80018F88` | `libgpu.c` | 0 | 0 | 7 |
| `func_80043F18` | `libgpu.c` | 0 | 0 | 18 |
| `func_80043EAC` | `libgpu.c` | 0 | 0 | 30 |
| `DrawSync` | `libgpu.c` | 44 | 1 | 30 |
| `LoadImage` | `libgpu.c` | 24 | 2 | 27 |
| `PutDrawEnv` | `libgpu.c` | 15 | 2 | 69 |
| `PutDispEnv` | `libgpu.c` | 15 | 5 | 335 |
| `MoveImage` | `libgpu.c` | 11 | 2 | 53 |

### `src/slus_006.64/psyq/libgte`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `NormalClip` | `libgte.c` | 10 | 0 | 15 |
| `CompMatrix` | `libgte.c` | 10 | 0 | 91 |
| `SetGeomScreen` | `libgte.c` | 8 | 0 | 7 |
| `RotAverage4` | `libgte.c` | 8 | 0 | 35 |
| `ScaleMatrix` | `libgte.c` | 8 | 0 | 78 |
| `SetGeomOffset` | `libgte.c` | 7 | 0 | 11 |
| `RotTransPers` | `libgte.c` | 7 | 0 | 15 |
| `func_80048C4C` | `libgte.c` | 7 | 0 | 42 |
| `func_8004A414` | `libgte.c` | 6 | 0 | 13 |
| `MulMatrix2` | `libgte.c` | 5 | 0 | 71 |

### `src/slus_006.64/psyq/libsn`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `PCread` | `libsn.c` | 7 | 1 | 55 |
| `PCwrite` | `libsn.c` | 2 | 1 | 55 |

### `src/slus_006.64/psyq/libspu`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `D_8001946C` | `Spu.c` | 0 | 0 | 7 |

### `src/slus_006.64/system/animation_scripts`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `SpriteSetScale` | `animation_scripts.c` | 2 | 0 | 17 |
| `func_80021C00` | `animation_scripts.c` | 1 | 0 | 10 |
| `func_80021BCC` | `animation_scripts.c` | 1 | 0 | 11 |
| `AnimScriptStackPushU24` | `animation_scripts.c` | 1 | 0 | 19 |
| `func_80022224` | `animation_scripts.c` | 1 | 0 | 41 |
| `func_80021EBC` | `animation_scripts.c` | 1 | 0 | 65 |
| `func_80022A00` | `animation_scripts.c` | 0 | 0 | 5 |
| `func_80021B04` | `animation_scripts.c` | 0 | 0 | 6 |
| `func_80021B14` | `animation_scripts.c` | 0 | 0 | 6 |
| `func_80021D3C` | `animation_scripts.c` | 0 | 0 | 7 |

### `src/slus_006.64/system/archive`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80029AFC` | `archive.c` | 9 | 14 | 261 |
| `func_80029EB0` | `archive.c` | 1 | 17 | 248 |

### `src/slus_006.64/system/graphics`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `GfxLoadImageAccelerated` | `graphics.c` | 1 | 3 | 27 |
| `GfxLoadClutsAccelerated` | `graphics.c` | 1 | 1 | 51 |

### `src/slus_006.64/system/libarchive`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8002945C` | `libarchive.c` | 1 | 0 | 26 |
| `func_80028E60` | `libarchive.c` | 1 | 0 | 33 |
| `func_8002954C` | `libarchive.c` | 1 | 2 | 39 |
| `func_80028B14` | `libarchive.c` | 1 | 6 | 231 |
| `func_800294B4` | `libarchive.c` | 0 | 1 | 45 |
| `func_80028F30` | `libarchive.c` | 0 | 4 | 352 |

### `src/slus_006.64/system/rendering`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8001F6B0` | `rendering.c` | 3 | 0 | 45 |
| `func_8001EE68` | `rendering.c` | 2 | 0 | 5 |
| `func_8001EE74` | `rendering.c` | 2 | 0 | 7 |
| `func_8001F530` | `rendering.c` | 1 | 0 | 38 |
| `func_8001F5BC` | `rendering.c` | 1 | 0 | 68 |
| `func_8001E148` | `rendering.c` | 3 | 4 | 91 |
| `func_8001E9BC` | `rendering.c` | 3 | 5 | 311 |
| `func_8001E298` | `rendering.c` | 2 | 3 | 27 |
| `func_8001E3D8` | `rendering.c` | 2 | 5 | 399 |
| `func_8001E2F8` | `rendering.c` | 1 | 3 | 31 |

### `src/slus_006.64/system/sound`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `SoundWaitSpuTransfer` | `sound.c` | 8 | 0 | 32 |
| `func_8003E5BC` | `sound.c` | 6 | 0 | 51 |
| `func_8003F43C` | `sound.c` | 2 | 0 | 13 |
| `func_8003A838` | `sound.c` | 2 | 0 | 30 |
| `func_8003A55C` | `sound.c` | 2 | 0 | 33 |
| `func_8003A344` | `sound.c` | 2 | 0 | 33 |
| `SoundEnableAllSpuChannels` | `sound.c` | 1 | 0 | 26 |
| `SoundMuteAllSpuChannels` | `sound.c` | 1 | 0 | 27 |
| `func_8003EEA0` | `sound.c` | 1 | 0 | 29 |
| `func_8003A5D0` | `sound.c` | 1 | 0 | 42 |

### `src/slus_006.64/system/temp1`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_800234AC` | `temp1.c` | 7 | 0 | 38 |
| `func_8002393C` | `temp1.c` | 3 | 0 | 7 |
| `func_80023440` | `temp1.c` | 2 | 0 | 13 |
| `func_80022CAC` | `temp1.c` | 2 | 0 | 16 |
| `func_80023468` | `temp1.c` | 2 | 0 | 23 |
| `func_80023804` | `temp1.c` | 2 | 0 | 81 |
| `func_80024730` | `temp1.c` | 2 | 0 | 118 |
| `GfxSetCurrentOT` | `temp1.c` | 1 | 0 | 6 |
| `func_80025180` | `temp1.c` | 1 | 0 | 21 |
| `func_80024FF4` | `temp1.c` | 1 | 0 | 22 |

### `src/slus_006.64/system/temp2`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_80030B14` | `temp2.c` | 2 | 0 | 78 |
| `func_8002DFE0` | `temp2.c` | 1 | 0 | 6 |
| `func_8002CCAC` | `temp2.c` | 1 | 0 | 9 |
| `func_8002DFF0` | `temp2.c` | 1 | 0 | 10 |
| `func_8002C6E0` | `temp2.c` | 1 | 0 | 10 |
| `func_80031804` | `temp2.c` | 1 | 0 | 11 |
| `func_800317E0` | `temp2.c` | 1 | 0 | 11 |
| `func_80030C40` | `temp2.c` | 1 | 0 | 17 |
| `func_8002CD24` | `temp2.c` | 1 | 0 | 19 |
| `func_800301C8` | `temp2.c` | 1 | 0 | 28 |

### `src/slus_006.64/system/temp3`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `func_8001BEEC` | `temp3.c` | 1 | 0 | 21 |
| `func_8001A684` | `temp3.c` | 1 | 0 | 31 |
| `func_8001AADC` | `temp3.c` | 1 | 0 | 114 |
| `func_8001C76C` | `temp3.c` | 0 | 0 | 94 |
| `func_8001B66C` | `temp3.c` | 5 | 2 | 23 |
| `func_8001BB50` | `temp3.c` | 2 | 2 | 25 |
| `func_8001B5E8` | `temp3.c` | 1 | 2 | 38 |
| `func_8001BE14` | `temp3.c` | 1 | 5 | 56 |
| `func_8001B844` | `temp3.c` | 1 | 8 | 68 |
| `func_8001C634` | `temp3.c` | 1 | 10 | 81 |

### `src/slus_006.64/system/work_list`
| Function | File | Callers | Callees | ASM Lines |
| :--- | :--- | :--- | :--- | :--- |
| `WorkListAddTask` | `work_list.c` | 3 | 0 | 40 |
| `WorkListRemoveTask` | `work_list.c` | 3 | 0 | 49 |
| `TimerWorkListRemoveTask` | `work_list.c` | 3 | 0 | 52 |
| `TimerWorkListAddTask` | `work_list.c` | 3 | 0 | 64 |
| `func_8001D298` | `work_list.c` | 2 | 0 | 5 |
| `WorkListsReset` | `work_list.c` | 2 | 0 | 10 |
| `func_8001D3F4` | `work_list.c` | 2 | 0 | 36 |
| `func_8001D2A4` | `work_list.c` | 1 | 0 | 5 |
| `func_8001D034` | `work_list.c` | 1 | 0 | 33 |
| `func_8001D164` | `work_list.c` | 0 | 0 | 18 |

## 👑 Most Highly Referenced Functions Overall
Functions called by the most other functions throughout the entire codebase.
| Function | Callers | Module | Matched? |
| :--- | :--- | :--- | :--- |
| `FieldScriptVMGetArgument` | 179 | `src/field/main/misc5.c` | ✅ |
| `FieldScriptVMGetInstructionArgument` | 116 | `src/field/main/misc5.c` | ✅ |
| `FieldScriptMemoryWriteU16` | 90 | `src/field/scripts/virtual_machine.c` | ✅ |
| `HeapAlloc` | 85 | `src/slus_006.64/system/memory.c` | ✅ |
| `HeapFree` | 82 | `src/slus_006.64/system/memory.c` | ✅ |
| `func_8009CFBC` | 55 | `src/field/scripts/variable_handlers.c` | ✅ |
| `v0` | 52 | `Unknown` | ✅ |
| `func_8009CF78` | 49 | `src/field/scripts/variable_handlers.c` | ✅ |
| `DrawSync` | 44 | `src/slus_006.64/psyq/libgpu.c` | ❌ |
| `FieldScriptVMGetActorIndex` | 39 | `src/field/main/misc.c` | ✅ |
| `FieldScriptVMGetVariableValue` | 38 | `src/field/scripts/virtual_machine.c` | ✅ |
| `ArchiveCdDataSync` | 37 | `src/slus_006.64/system/libarchive.c` | ✅ |
| `func_8009D000` | 37 | `src/field/scripts/variable_handlers.c` | ✅ |
| `ArchiveSetIndex` | 29 | `src/slus_006.64/system/libarchive.c` | ✅ |
| `Vsync` | 28 | `src/slus_006.64/psyq/libetc/vsync.c` | ✅ |
| `ArchiveDecodeAlignedSize` | 28 | `src/slus_006.64/system/libarchive.c` | ✅ |
| `GetTPage` | 28 | `src/slus_006.64/psyq/libgpu.c` | ✅ |
| `SetRotMatrix` | 26 | `src/slus_006.64/psyq/libgte.c` | ✅ |
| `SetTransMatrix` | 26 | `src/slus_006.64/psyq/libgte.c` | ✅ |
| `LoadImage` | 24 | `src/slus_006.64/psyq/libgpu.c` | ❌ |