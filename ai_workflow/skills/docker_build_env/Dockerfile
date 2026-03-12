FROM ubuntu:latest

# Install build dependencies required by Makefile, Splat, and objdiff
RUN apt-get update && apt-get install -y \
  make \
  binutils-mips-linux-gnu \
  cpp-mips-linux-gnu \
  python3 \
  python3-pip \
  python3-venv \
  curl \
  git \
  build-essential \
  p7zip-full \
  && rm -rf /var/lib/apt/lists/*

# Fix python symlink
RUN ln -s /usr/bin/python3 /usr/bin/python

# Create a workdir
WORKDIR /app

# Ensure pip doesn't complain about running as root globally if we run python packages
ENV PIP_BREAK_SYSTEM_PACKAGES=1

# Copy project requirements directly in build (since they rarely change)
COPY requirements.txt /tmp/requirements.txt
RUN pip install -U -r /tmp/requirements.txt

# Keep the container running in the background indefinitely
CMD ["tail", "-f", "/dev/null"]
