---
name: Docker Environment Wrapper
description: Provides commands to build the PS1 decomp toolchain using a local Docker daemon
---

# Docker Build Environment Skill

This skill allows the agent to execute Linux-specific compilation tools (`mips-linux-gnu`, `splat`, `objdiff`) locally on a Mac by routing them through a Docker container.

## Architecture
The repository's build system requires `cpp-mips-linux-gnu` and `binutils-mips-linux-gnu`. Since you are running on a Mac, you cannot invoke these natively. We use a background Docker container (`xenogears_decomp_env`) mapping the current directory (`/app`).

## Setup 
To build the image and start the background development daemon:
```bash
docker build -t xenogears_builder ./ai_workflow/skills/docker_build_env
docker run -d --name xenogears_decomp_env -v $(pwd):/app xenogears_builder
```

## How to Execute Makefile Commands
Whenever you need to run `make`, `python -m splat`, or `objdiff`, you must pipe the command through `docker exec`:

### Example: Running Make
```bash
docker exec -it xenogears_decomp_env make build
```

### Example: Checking Diff
```bash
docker exec -it xenogears_decomp_env make report
```
