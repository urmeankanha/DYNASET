# DYNASET — Game Calculator Suite

A native C++ desktop app built with **Dear ImGui + SDL2 + OpenGL3**. Designed to be fast, offline, and expandable.

---

##  Current Calculators

| Calculator | Description |
|---|---|
| **Spring & Damping** | Computes ζ, ωn, ωd, settling time, overshoot, and live step-response graph |

---

##  Setup

### Prerequisites

**Linux (Ubuntu/Debian)**
```bash
sudo apt install build-essential libsdl2-dev libgl-dev cmake
```

**macOS (Homebrew)**
```bash
brew install sdl2 cmake
```

**Windows**
1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with C++ Desktop workload
2. Install [SDL2 development libraries](https://www.libsdl.org/release/) to `C:\SDL2\`
3. Use the CMake path below

---

##  Build & Run

### Linux / macOS (Make)
```bash
make -j4
./devtools_calc
```

### All Platforms (CMake)
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
./build/devtools_calc      # Linux/macOS
build\Release\devtools_calc.exe   # Windows
```
##  Adding a New Calculator

1. **Define a state struct** in `src/app.h` (like `SpringState`)
2. **Add an entry** to `GetCalculatorRegistry()` in `src/app.h`
3. **Add a new `AppPage` enum value** in `src/app.h`
4. **Create your logic header** (like `src/spring_calc.h`)
5. **Create your page UI** (like `src/page_spring.h`)
6. **Add `#include` and dispatch** in `src/main.cpp`

That's it — the landing page auto-discovers the registry.

---

## ✦ Architecture

```
SpringCalc/
├── src/
│   ├── main.cpp          ← Entry point, SDL2/GL loop, page dispatch
│   ├── app.h             ← AppState, CalculatorRegistry, enums
│   ├── ui_theme.h        ← ImGui theme, helper widgets (Badge, StatCard, etc.)
│   ├── spring_calc.h     ← Spring physics math, step response generator
│   ├── page_landing.h    ← Landing page UI
│   └── page_spring.h     ← Spring damping calculator UI
├── libs/
│   └── imgui/            ← Dear ImGui (cloned via git)
├── .vscode/
│   ├── tasks.json        ← Build/Run tasks
│   ├── launch.json       ← Debug configs
│   └── c_cpp_properties.json ← IntelliSense paths
├── Makefile              ← Linux/macOS quick build
└── CMakeLists.txt        ← Cross-platform build
```

---
