# Journal Entry: 2026-03-12 23:34 - Fast Incremental Builds and Additional Matches

## Objective
To test and validate a fast incremental build loop using `make build` instead of full rebuilds (`make clean-build`) in order to drastically reduce the compilation feedback loop time, and to apply this methodology to match additional target leaf functions.

## Actions Taken
1.  **Tested `make build` Speed**:
    - Discovered that `make report` artificially strips `.o` files (for `objdiff` comparison) and taints the compiler cache for subsequent developer `make build` attempts, causing link errors.
    - Resolved this by running one final `make clean-build` to restore the cache. Subsequent edits evaluated with `make build` alone compiled single modified files in **~0.5 seconds**.
2.  **Matched `func_80021BF8`**:
    - Identified via `find_target.py` as a 4-line `sw` (store word) leaf function in `asm/slus_006.64/nonmatchings/system/animation_scripts/`.
    - Translated to C: `void func_80021BF8(SpriteData* pSpriteData, void* cb) { pSpriteData->field_0x68 = cb; }` inside `src/slus_006.64/system/animation_scripts.c`.
    - Compiled instantly via incremental `make build`. Verified hash.
3.  **Matched `func_80023950`**:
    - Identified via `find_target.py` as another 4-line leaf function in `asm/slus_006.64/nonmatchings/system/temp1/`.
    - Translated to C: storing zero to offset `0x20` of `SpriteData`. `0x20` is `SpriteDataI1* pBase`, so the code is `pSpriteData->pBase = NULL;`.
    - *Blocker*: The fast incremental build caught a missing `#include "field/actor.h"` header in `src/slus_006.64/system/temp1.c` in under 0.3 seconds.
    - Fixed the header and compiled perfectly. Verified hash.
4.  **Updated Global Progress**:
    - Ran the full hollowed `make report` cycle to regenerate `build/progress.json`.
    - The repository's "Matched Functions" count successfully increased from 673 to **676**!

## Meta-Learnings & DKB Updates
- **Incremental Cache Validation**: Do not use `make report` or `make check` during rapid trial-and-error AI iteration loops. They overwrite development object files. The orchestrator script should exclusively rely on `make build` (which completes in milliseconds if only one C file is touched) and raw `objdiff` binary inspections until a function is structurally 100% matched.
- Missing `#include` dependencies are caught almost instantly by `cc1` using this methodology.
- The `ai_workflow/knowledge_base.json` has been updated with this specific makefile caching heuristic.

## Next Logical Steps
With the rapid feedback loop empirically validated and three independent leaf functions securely checked-in natively, we are fully prepared to build the `auto_decomp.py` brain to continuously execute this exact workflow autonomously over all 1,260+ remaining unmatched functions.
