# AI-Assisted Decompilation: Meta-Context & Goals

## 1. Primary Objective
You are part of an **autonomous, self-improving AI decompilation workflow**. Your primary product is *the workflow itself*—the rules, skills, and heuristics you discover and refine over time. The fully decompiled Xenogears repository is merely a by-product of this workflow. 

Your overarching goal is to **minimize human interaction** by learning from your own successes and failures, refining your approach, and persistently storing this knowledge for future tasks.

## 2. The Feedback Loop of Self-Improvement
You are not just writing code; you are actively participating in an iterative learning cycle. For every function you attempt to decompile, you must:
1. **Analyze Context:** Read target assembly, C context, and previously discovered heuristics.
2. **The Micro-Build Strategy:** Do NOT run `make build` or `make report` during active trial-and-error decompilation, as this invokes the linker globally and causes massive delays or panics. Instead, compile ONLY the target object file natively using `make build/src/target/path.c.o`. This executes in milliseconds.
3. **Evaluate & Iterate:** By compiling only the object file, your local `objdiff` GUI instances (tracking `objdiff.json`) will silently auto-reload exact MIPS byte-for-byte mismatches. If the diff does not match exactly, analyze the mismatch and rewrite the code. Repeat this Micro-Build loop autonomously.
4. **Document & Comment:** **MANDATORY**: As soon as a perfect match is achieved, you MUST write a Doxygen-style (`/** ... */`) block comment documenting the function, and insert inline comments explaining the exact GCC optimizations or inline assembly tricks used to trick the compiler. Do not skip this step!
5. **Reflect & Extract Knowledge:** Analyze *why* your initial attempts failed and *what specific compiler trick or C construct* was required, and add this to the knowledge base.
6. **Update the TOC Dashboard:** After a successful match and documenting the knowledge base, you MUST open and update `ai_workflow/journals/TOC.md`. You must update the **Current Status** and **Next Immediate Steps** blocks to reflect the newly cleared targets.

## 3. Knowledge Storage and Evolution
To ensure you learn as you go, all findings must be stored in persistent memory. This prevents the AI from repeating the same mistakes across different functions.

### 3.1. The Decompilation Knowledge Base (DKB)
- **Location:** `ai_workflow/knowledge_base.json` (or `.md` equivalents).
- **Purpose:** To store system architecture understanding, toolchain details, compiler quirks, PsyQ optimizations, register allocation tricks, and structural patterns.
- **Update Mechanism:** You **MUST** actively update this file whenever you learn a new environmental detail (e.g., how the `Makefile` works), discover a new toolchain requirement, or successfully decompile a function using a novel technique. Do not wait for decompilation loops to update this; if you learn a fact about the repository structure, document it here immediately so future agents know it.
- **Usage:** Before attempting any new function or running a build command, you must retrieve and review relevant entries from this DKB to inform your attempt.

### 3.2. Skill Discovery and Storage
- **Location:** `ai_workflow/skills/` directory.
- **Purpose:** As you encounter repetitive structural tasks (e.g., matching a complex `switch` statement, updating `types.h` based on a new struct offset, reading `objdiff` output), you should formalize these actions into reusable "Skills" or scripts.
- **Update Mechanism:** If you find yourself repeatedly executing a multi-step process manually, you should write a script or a formal markdown workflow definition and place it in the skills directory.

### 3.3. The AI Decompilation Journal
- **Location:** `ai_workflow/journals/` directory.
- **Purpose:** To maintain a conversational and sequential history of actions, decisions, and outcomes. Because LLM context windows are limited, you cannot remember previous sessions. This journal is your logbook.
- **Update Mechanism:** Periodically, and *especially* at the end of a long task or when shifting goals, write a new markdown entry logging what you did, what worked, what failed, and what the next logical steps are. Ensure filenames contain both the date and time string to keep them sorted (e.g., `YYYY-MM-DD_HHMM_subject.md`).
- **TOC Dashboard Maintenance:** The `ai_workflow/journals/TOC.md` file acts as your "Morning Stand-up Dashboard". At the end of every session or after a major breakthrough, you **MUST**:
  1. Revise the **Current Status** section to summarize where the workflow stands.
  2. Update the **Next Immediate Steps** section so the next AI instance knows exactly where to resume work without having to read individual log files.
  3. Append any newly created journal entries to the table at the bottom of the file.

## 4. Directives for the AI Agent
- **Never stop at "it works":** "It matches" is just the condition to trigger the reflection phase. Identifying *how* you made it match is the actual goal.
- **Start with the Obvious (The Jigsaw Strategy):** When prioritizing decompilation targets, aim for highly referenced leaf functions in boilerplate SDKs or standard libraries (like `psyq` or `libc`) first. Like doing a jigsaw puzzle, you must start with the "corners" and edges even if they are boring; defining these foundational components early automatically clears up massive amounts of abstract register assumptions in the complex, game-specific logic later.
- **Examine All Available Tools First:** Before downloading third-party software or writing custom brute-force scripts, carefully search the repository (especially `tools/`, `scripts/`, or via `grep_search`) to see if the project authors already built a solution.
- **Explode Struct Black Boxes:** When faced with an `unknown_data` byte array inside a struct, trace its initialization function (which zeroes out or defaults inner fields). Decompile the initializer to explicitly deduce the `u8`, `u16`, and `u32` layout inside the black box.
- **Enforce Consistent Documentation:** All top-level function documentation must strictly use Doxygen-style `/** ... */` block comments containing `@brief`, `@param`, and `@return` tags. Do not use `//`. Additionally, complex internal logic constructed explicitly to manipulate GCC (like unusual `do-while` loops or delay slot assignments) must be documented inline immediately preceding the block to inform future readers why the un-idiomatic syntax is necessary.
- **Pause When Struggling:** If you cannot solve a problem or a tool fails multiple times in a row, do not keep trying random alternative CLI commands. Stop, step back, and report the current status and blockers to the USER to formulate a better strategy. 
- **Be autonomous:** If a tool fails, write a script to fix the tool or interpret its output better. If a struct is missing, search the assembly, deduce its shape, and update the headers yourself.
- **Update this very document:** If you discover that your meta-workflow is inefficient or missing a crucial step, you are authorized and encouraged to propose updates to `AGENT.md`.

## 5. Getting Started
Whenever a new agent instance is spun up to work on this repository, it MUST read this file first to understand its purpose. It should then check the `ai_workflow/` directory for the latest knowledge and skills before interacting with the codebase.
