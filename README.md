# Snake — DSA Edition 🐍

A Snake game built in **C++17 + SFML**, written from the perspective of a
4th-semester Data Structures & Algorithms student.  Every core mechanic is
backed by an explicit DSA implementation — not just STL wrappers.

---

## Data Structures Used

| Structure | Location | Role | Complexity |
|---|---|---|---|
| **Doubly Linked List** | `src/ds/DoublyLinkedList.h` | Snake body (segments) | `push_front` / `pop_back` O(1) |
| **Circular Queue** | `src/ds/CircularQueue.h` | Input buffering (direction keys) | `enqueue` / `dequeue` O(1) |
| **2-D Grid (static array)** | `src/ds/Grid.h` | Occupancy map — collision detection | Lookup O(1) |

---

## Project Structure

```
SnakeGame/
├── CMakeLists.txt
├── assets/
│   └── fonts/          ← drop Roboto-Regular.ttf here (optional, see below)
└── src/
    ├── main.cpp
    ├── core/
    │   ├── Constants.h  — grid/window/timing constants
    │   ├── GameState.h  — shared enum
    │   ├── Snake.h/.cpp — uses DoublyLinkedList + CircularQueue
    │   ├── Food.h/.cpp  — random respawn via occupancy Grid
    │   └── Game.h/.cpp  — main loop, state machine, collision
    ├── ds/
    │   ├── DoublyLinkedList.h
    │   ├── CircularQueue.h
    │   └── Grid.h
    └── ui/
        ├── Renderer.h
        └── Renderer.cpp — all SFML drawing, dark theme, animations
```

---

## Prerequisites

### 1 — Install SFML 2.5+

**Windows (vcpkg)**
```
vcpkg install sfml
```

**macOS (Homebrew)**
```
brew install sfml
```

**Ubuntu / Debian**
```
sudo apt install libsfml-dev
```

### 2 — (Optional) Add a font

Download **Roboto-Regular.ttf** from [Google Fonts](https://fonts.google.com/specimen/Roboto)
and place it at:
```
SnakeGame/assets/fonts/Roboto-Regular.ttf
```
If missing, the game auto-falls back to a system font (Arial / DejaVu Sans).

---

## Building in CLion

1. Open CLion → **File → Open** → select the `SnakeGame/` folder.
2. CLion detects `CMakeLists.txt` automatically.
3. If using vcpkg on Windows, set the CMake toolchain file in
   **Settings → Build → CMake → CMake options**:
   ```
   -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```
4. Click **Build** (hammer icon) then **Run**.

---

## Controls

| Key | Action |
|---|---|
| `W` / `↑` | Move Up |
| `S` / `↓` | Move Down |
| `A` / `←` | Move Left |
| `D` / `→` | Move Right |
| `P` / `Esc` | Pause / Resume |
| `Enter` | Start / Restart |

---

## Game Features

- **Gradient snake** — head is bright neon-green, body fades to dark green
- **Pulsing food** — glow rings animate continuously via SFML Clock
- **Speed ramp** — every food eaten shaves ~23 ms off the tick interval
- **Persistent high score** — survives restarts within the same session
- **Input buffering** — circular queue holds up to 4 ahead-queued turns
