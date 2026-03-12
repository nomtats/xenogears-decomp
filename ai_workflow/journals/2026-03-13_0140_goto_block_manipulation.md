# 2026-03-13: Flat Goto Block Manipulation for Branch Polarity

## Objective
Decompile `func_8003E290` (now renamed `SoundCalculateEnvelopeStep`).

## Actions Taken
1. Ran `find_target.py` on the `sound/` module, identifying `func_8003E290` as the highest caller-density unknown leaf function (18 callers, 36 instructions).
2. Looked at the assembly and recognized the arithmetic (division by divisors and divisor-1) representing an ADSR envelope or pitch slide math function.
3. Used `rename_symbol.py` to globally map `func_8003E290` -> `SoundCalculateEnvelopeStep` across the project, including injecting it into `config/symbol_addrs.slus_006.64.txt`.

## The GCC `bnez` / `beqz` Branch Polarity Puzzle (Crucial Heuristic)
My natural attempt to write standard nested C `if` / `else if` blocks failed byte matching spectacularly.

### The Problem
GCC compiles `if (a2 < 4) { do_div(); } else { do_something_else(); }` by:
1. Emitting the condition `slti v0, a2, 4`.
2. Emitting an inverted branch `beqz v0, else_block` (branch to the else block if the condition is FALSE).
3. The `if` block immediately follows the branch physically in memory.

However, the original assembly emitted a direct true branch: `bnez v0, do_div`, and fell through to the else logic! 
Because the structure of the C code fundamentally dictates whether GCC builds a fall-through or an explicit jump, standard control loops will absolutely fail to match `bnez` forward references.

### The Solution: Flat Goto Manipulation
To perfectly force GCC 2.7.2 to emit explicit true-branches (`bnez v0, label_x` or `beq a1, v0, label_y`), you must *abandon standard C structure* and simulate raw basic blocks using flat `goto` instructions:

```c
    if (a2 < 4) goto block_div;
    if (a2 == 4) goto block_div_minus_1;
    goto end;

block_div:
    targetDelta = targetDelta / envelopeBase;
    goto end;

block_div_minus_1:
    if (envelopeBase == 1) goto end;
    targetDelta = targetDelta / (envelopeBase - 1);

end:
    return targetDelta;
```

This perfectly matched the assembly by completely disentangling the blocks from the condition evaluating them.

## Important Discovery regarding `asm/` files and Git
I discovered that `asm/` is declared in `.gitignore`. The `make setup` / `make generate` commands entirely wipe and recreate the contents of `asm/` continuously using `splat`. This means manual `git rm asm/xyz.s` commands are categorically unnecessary. Adding a new symbol name to `config/symbol_addrs.slus_006.64.txt` natively commands `splat` to deploy the new name globally on the next `make clean-build`.
