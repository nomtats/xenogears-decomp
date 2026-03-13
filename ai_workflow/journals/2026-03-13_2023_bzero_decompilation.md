# AI Decompilation Journal: bzero Perfect Match (libc.c)

**Date:** 2026-03-13
**Time:** 20:23
**Target:** `bzero` (Standard Library, PsyQ `libc.c`)

## Context & Objective
Following the success of perfectly matching `memcpy`, the next highly-referenced leaf node on the Jigsaw Strategy target list was `bzero`. This standard library function shares structural characteristics with `memcpy`, specifically precise compiler packing utilizing branch delay slots. 

## The Challenge
The MIPS representation for `bzero` was extremely tiny (17 instructions), however `GCC 2.7.2` repeatedly refused to structurally bundle the return assignments correctly. The obstacles were:

1. **Branch Delay Slots & Merging:** GCC attempted to crossjump (block merge) the early-exit `return NULL` cases, which implicitly broke the explicit variable assignments happening inside the branch delay slots natively produced by PsyQ's compiler.
2. **Branch Polarity (`blez` vs `bgtz`):** C's standard structure compiles as inverted conditions (`blez`). We needed to force a raw `bgtz` true-branch.

## Actions Taken
1. Set up the fast Micro-Build feedback loop for `build/src/slus_006.64/psyq/libc.c.o`.
2. Recognized the issue from the Knowledge Base logic: **Heuristics `#10` (Flat Goto Manipulation) and `#12` (Branch Delay Slot Variable Placement)**.
3. Instead of nested `if/else` and `do/while` loops, rewrote `bzero` using flat `goto` basic blocks to explicitly route branch directions preventing crossjumping.
4. Altered the local stack variables mapping. Initializing a single `start = NULL` dynamically allowed the delay slot evaluation block to explicitly fall through into `return NULL` correctly without executing redundant register `move` assignments.

## The Final Output Code
To achieve the exact delay slot structure, the function is coded functionally identically to a state-machine relying on flat `goto`:

```c
void* bzero(u_char* pDst, int size) {
    u_char* start;

    if (pDst == NULL) {
        return NULL;
    }

    if (size > 0) {
        start = pDst;
        goto loop;
    }
    
    start = NULL;
    goto end;

loop:
    do {
        *pDst = 0;
        size--;
        pDst++;
    } while (size > 0);

end:
    return start;
}
```

## Outcomes
- Perfect byte-for-byte MIPS translation for `bzero`. 
- Refined the methodology for manipulating ancient GCC compilation behaviors to match handwritten or heavily-optimized basic blocks.

## Next Steps
Continue chipping away at similar standard leaf routines inside `libc.c` (e.g., `memset`, which shares identical logic constraints as `memcpy` and `bzero`).
