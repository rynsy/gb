# Gameboy Emulator

[![CI](https://github.com/rynsy/gb/actions/workflows/ci.yml/badge.svg)](https://github.com/rynsy/gb/actions/workflows/ci.yml)
[![Pages](https://github.com/rynsy/gb/actions/workflows/pages.yml/badge.svg)](https://github.com/rynsy/gb/actions/workflows/pages.yml)
[![Release](https://github.com/rynsy/gb/actions/workflows/release.yml/badge.svg)](https://github.com/rynsy/gb/actions/workflows/release.yml)

## Getting Started

### Clone

```bash
git clone https://github.com/rynsy/gb.git
cd gb
git submodule update --init --recursive
```

---

## Native Development (Linux)

### Dependencies

Install the required toolchain + X11/OpenGL dev headers:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake git python3 \
  libasound2-dev libgl1-mesa-dev \
  libx11-dev libxrandr-dev libxi-dev \
  libxinerama-dev libxcursor-dev libxfixes-dev \
  libxrender-dev libxkbcommon-dev
```

### Build (Desktop)

```bash
make build-desktop
```

### Run (Desktop)

```bash
make run-desktop
```

### Run Tests

```bash
make test
```

---

## WASM Development

### Install Emscripten

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ..
```

### Build WASM

```bash
make build-wasm
```

### Run WASM Locally (Simple HTTP Server)

```bash
make run-wasm
```

This serves the WASM bundle at:

http://localhost:8000/index.html


---

## Using the Dev Container (Optional)

This provides a fully reproducible environment for building and testing.

### Build the container

```bash
docker build -t gbemu-dev .
```

### Enter the container

```bash
docker run --rm -it -v "$PWD":/src -w /src gbemu-dev
```

Inside the container:

```bash
git submodule update --init --recursive
make build-desktop
make test
make build-wasm
```

> Note: GUI desktop build **cannot** run inside Docker without X forwarding.
> Use your host machine for `make run-desktop`.

---

## CI/CD

- **CI** builds & tests on each commit.
- **GitHub Pages** publishes the WASM build.
- **Releases** are created automatically when tagging:

```bash
git tag v0.1.0
git push origin v0.1.0
```

Desktop (Windows/Linux) and WASM artifacts appear on the release page.

