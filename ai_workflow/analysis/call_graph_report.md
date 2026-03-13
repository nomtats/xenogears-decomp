# Xenogears Call Graph Analysis

## Top 20 Candidates for Decompilation
These are unmatched functions, prioritized strictly by 'Leaf' status and then 'Incoming References'.
| Function | Callers | Is Leaf |
| --- | --- | --- |
| `DrawSync` | 13 | True |
| `func_80072254` | 13 | True |
| `LoadImage` | 8 | True |
| `PCclose` | 8 | True |
| `PutDrawEnv` | 7 | True |
| `PutDispEnv` | 7 | True |
| `PCopen` | 5 | True |
| `func_8009E574` | 4 | True |
| `DrawOTag` | 4 | True |
| `PClseek` | 4 | True |
| `PCinit` | 4 | True |
| `func_80085634` | 4 | True |
| `MoveImage` | 3 | True |
| `StoreImage` | 3 | True |
| `ChangeClearPAD` | 3 | True |
| `func_80039FF8` | 3 | True |
| `ClearImage` | 3 | True |
| `func_8003B644` | 3 | True |
| `PCread` | 3 | True |
| `WorkListRemoveTask` | 2 | True |

## Most Highly Referenced Functions Overall (Matched & Unmatched)
Functions that are called the most throughout the codebase.
| Function | Callers | Matched? |
| --- | --- | --- |
| `FieldScriptVMGetInstructionArgument` | 46 | ✅ |
| `FieldScriptVMGetArgument` | 37 | ✅ |
| `FieldScriptMemoryWriteU16` | 35 | ✅ |
| `HeapFree` | 29 | ✅ |
| `FieldScriptVMGetVariableValue` | 27 | ✅ |
| `v0` | 22 | ✅ |
| `HeapAlloc` | 20 | ✅ |
| `Vsync` | 19 | ✅ |
| `func_8009CFBC` | 15 | ✅ |
| `CdSyncCallback` | 13 | ✅ |
| `DrawSync` | 13 | ❌ |
| `func_80072254` | 13 | ❌ |
| `.text` | 12 | ✅ |
| `FieldScriptVMGetActorIndex` | 12 | ✅ |
| `CdReadyCallback` | 10 | ✅ |
| `printf` | 10 | ✅ |
| `SoundHandleError` | 10 | ✅ |
| `FieldScriptVMGetInstructionArgumentS16` | 10 | ✅ |
| `ExitCriticalSection` | 9 | ✅ |
| `puts` | 9 | ✅ |