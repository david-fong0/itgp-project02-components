#pragma once
#include "GameObject.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>

class Engine {
public:
    static bool init(const char* title, int width, int height);
    static void run();
    static void shutdown();
    static SDL_Renderer* renderer();
    static int width(), height();
    static void add(std::unique_ptr<GameObject> object);
    static std::vector<std::unique_ptr<GameObject>>& objects();
    static GameObject* player();
    static void setPlayer(GameObject* p);
    static void removeDeadObjects();
    static void drawWorld();
    static void setMessage(std::string message);
    static float deltaTime();
    static const std::string& message();
private:
    static SDL_Window* window_;
    static SDL_Renderer* renderer_;
    static bool running_;
    static int width_, height_;
    static std::vector<std::unique_ptr<GameObject>> objects_;
    static GameObject* player_;
    static std::string message_;
    static float deltaTime_;
};
