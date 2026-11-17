# Escape Through Time — Component-Based Game Demo

A small SDL2 2D action-platformer prototype built around a component-based game architecture. The project adapts the *Escape Through Time* premise into one compact playable level, **Buried Treasure**.

## Features

- **Component-based architecture:** Each `GameObject` owns a vector of components.
- **Component interface:** Abstract `Component` class with `update()`, `draw()`, and `parent()` functions.
- **BodyComponent:** Handles position, velocity, angle, size, gravity, and grounded state.
- **SpriteComponent:** Draws game objects using SDL2, including loaded image textures.
- **AssetManager:** Loads BMP images from the assets folder and stores textures in a `std::map` using their filenames as keys.
- **View class:** Implements a camera that follows the player and transforms world coordinates into screen coordinates.
- **ControllerComponent:** Supports keyboard movement, jumping, and sword attacks.
- **PatrolComponent:** Allows enemies to patrol between horizontal boundaries.
- **ChaseComponent:** Allows enemies to pursue the player within a detection radius.
- **CombatComponent:** Handles player and enemy health, contact damage, attack cooldowns, and enemy defeat.
- **ComponentFactory:** Reads YAML configuration and creates components using a registry map.
- **Frame rate management:** Supports a configurable target FPS and waits when frames finish early.
- **Delta time:** Tracks elapsed frame time for consistent movement and physics.
- **HUD:** Displays the player's health.

## Controls

- **A / D** or **Left / Right:** Move
- **Space**, **W**, or **Up:** Jump
- **J:** Sword attack
- **Esc:** Quit

## Build and Run (VS Code + MSYS2 UCRT64)

Install the required packages in the MSYS2 UCRT64 terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-yaml-cpp pkgconf make
```

Open a terminal in the project directory and run:

```bash
make
./EscapeThroughTime.exe
```

Run the program from the project root because it loads `assets/level1.yaml` and image files from the relative `assets` directory.

## Configuration

The target frame rate can be changed in `main.cpp`:

```cpp
Engine::setTargetFPS(60);
```

The number can be changed to another positive value to adjust the target FPS.

The AssetManager automatically scans the `assets` directory for BMP files. Each image is stored using its filename without the extension as its key. For example, `player.bmp` can be retrieved using:

```cpp
AssetManager::get("player");
```

The level configuration is stored in `assets/level1.yaml`, where game objects and their components are defined.

## Architecture Notes

`GameObject::add<T>(args...)` constructs a component with its owning `GameObject` and appends it to the component vector. `get<T>()` searches by dynamic type and returns a non-owning pointer or `nullptr`.

The `ComponentFactory` uses a registry map to associate