```markdown
# 🛠️ Build and Run Guide - LAN Checkers

## Prerequisites

- Linux system (current release supports Linux only).
- `gcc` and `make` installed.
- [Raylib](https://www.raylib.com/) installed and available system-wide.

## Building the Game

Open a terminal in the root directory of the project and run:

```bash
make
```

This compiles the source code into the executable.

## Running the Game

To compile and run in one step:

```bash
make run
```

## Additional Commands

| Command         | Description                         |
|-----------------|-----------------------------------|
| `make clean`    | Remove compiled files and binaries |
| `make debug`    | Launch game under GDB (debugger)  |

## Installing Raylib on Linux (Ubuntu/Debian)

```bash
sudo apt install libraylib-dev
```

Or build from source by following instructions on the [Raylib official site](https://www.raylib.com/).

## Notes

- The executable will be created in the `./bin/` directory.
- Debug logs are written to `./debug/debug.log` when debug mode is enabled.
```
