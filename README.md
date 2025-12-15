```markdown
# LAN Checkers (Draughts)

A simple but functional Checkers (Draughts) game for Linux, featuring two gameplay modes:

- 🧠 Single-player vs CPU  
- 🌐 LAN Multiplayer (host or connect to a game on the same network)  

> ⚠️ This release targets **Linux only**. Windows support is planned for the next release.

The game follows classic International Draughts rules, including pawn promotion to kings and multi-jump captures. Turns alternate between players with clear visual indication. In LAN mode, one player hosts and sees their local IP; the second player connects using that IP — both must be on the same network.

## 🖱️ Controls

| Action               | Key / Input   |
|----------------------|----------------|
| Select / Move Piece  | Mouse Click    |
| Toggle Music         | `Spacebar`     |
| Toggle Debug Logging | `F1`           |
| Exit Game            | `ESC`          |

Debug logs are saved to `./debug/debug.log`.

## 🛠️ Building & Running

### Requirements

- A Linux system with `make` and `gcc`

### To Install:
- git clone --recurse-submodules https://github.com/ArnoldFromancius/lan-checkers-game.git 

### Or if already cloned:
- git submodule update --init --recursive

### Install Dependencies:
    sudo apt install -y \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libgl1-mesa-dev

### Quick Start

```bash
make run
```

Other useful commands:
```bash
make        # Compile only  
make clean  # Clean build files  
make debug  # Run in GDB TUI mode
```

## 📦 Dependency: Raylib

Raylib is a lightweight C library for creating 2D/3D games. It handles:

- Windowing and drawing
- Input handling (mouse, keyboard)
- Audio playback

To install on Debian/Ubuntu:

```bash
sudo apt install libraylib-dev
```

Or build from source: https://www.raylib.com/

## 📅 Roadmap

- ✅ CPU vs Player mode  
- ✅ LAN multiplayer  
- 🚧 Windows support  
- 🚧 Improved AI  
- 🚧 UI/UX and animations  
- 🚧 Sound/music toggles in-game  

## 📝 License

This project is released for educational/personal use. A formal license will be added in a future version.

## 🧠 Author Notes

This project was created as part of a learning journey in:

- C programming
- Computer networking
- Cross-platform game development
- Git and debugging workflows
- Writing documentation
```
