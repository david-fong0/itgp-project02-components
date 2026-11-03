#pragma once
#include <SDL.h>
#include <set>

class Input {
public:
    static void processEvent(const SDL_Event& event);
    static bool down(SDL_Scancode key);
    static bool pressed(SDL_Scancode key);
    static void endFrame();
private:
    static std::set<SDL_Scancode> held_, pressed_;
};
