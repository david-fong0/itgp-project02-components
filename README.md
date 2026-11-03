# Escape Through Time — Component-Based Game Demo

A small SDL2 2D action-platformer prototype for the component architecture assignment. It adapts the *Escape Through Time* premise from the accompanying game design document into one compact playable level, **Buried Treasure**.

## Features

- Component-based `GameObject` architecture: each object owns a `std::vector<std::unique_ptr<Component>>`.
- Abstract `Component` interface with `update()`, `draw()`, and `parent()`.
- `BodyComponent`: position, velocity, angle, size, and grounded state.
- `SpriteComponent`: draws simple colored character sprites using SDL2 primitives (no external art assets required).
- `ControllerComponent`: keyboard movement, jumping, and sword attacks; attacks work while moving.
- `PatrolComponent` (`Behavior1Component`): patrols between two horizontal bounds.
- `ChaseComponent` (`Behavior2Component`): pursues the player inside a detection radius.
- `CombatComponent`: player/enemy health, contact damage, attack cooldown, and enemy defeat.
- `ComponentFactory`: reads YAML and uses `std::map<std::string, std::function<...>>` to create and attach components.
- Simple HUD health bar and a small platform layout.

## Controls

- **A / D** or **Left / Right**: move
- **Space**, **W**, or **Up**: jump
- **J**: sword attack (can attack while moving)
- **Esc**: quit

## Build (VS Code + MSYS2 UCRT64)

Install the UCRT64 packages in the MSYS2 UCRT64 terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-yaml-cpp pkgconf make
```

From the project directory:

```bash
make
./EscapeThroughTime.exe
```

The program loads `assets/level1.yaml` relative to the working directory, so run it from the project root. In VS Code, choose **Terminal → New Terminal**, select **MSYS2 UCRT64**, then run the commands above.

Alternative CMake build:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
./build/EscapeThroughTime.exe
```

## Architecture notes

`GameObject::add<T>(args...)` constructs a component with its owning `GameObject&` and appends it to the component vector. `get<T>()` searches by dynamic type and returns a non-owning raw pointer or `nullptr`. `update()` and `draw()` forward to every component.

The factory is initialized with a registry map. Each registry entry maps a YAML `type` name to a creator function. The factory reads each object's `components` sequence and invokes the matching creator, so new component types can be registered without adding new `GameObject` subclasses.

`Behavior1Component` and `Behavior2Component` are aliases for the named behaviors used in this demo: patrol and chase. The concrete classes are `PatrolComponent` and `ChaseComponent`.

## Current prototype scope

This is intentionally a one-level mechanics demo, not a complete metroidvania. It uses simple rectangle-based artwork and lightweight platform/floor handling rather than a full physics engine. The body component handles gravity and landing on the floor and three simple platforms; this is intentionally lightweight rather than a full physics engine. The second-level boss, save system, and full animation/audio pipeline from the design document are outside this assignment prototype's current scope.

## Screenshots

Add screenshots after running the demo:

```markdown
![Gameplay screenshot](docs/gameplay.png)
```
