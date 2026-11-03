#include "Input.h"
std::set<SDL_Scancode> Input::held_;
std::set<SDL_Scancode> Input::pressed_;
void Input::processEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && !e.key.repeat) {
        held_.insert(e.key.keysym.scancode);
        pressed_.insert(e.key.keysym.scancode);
    } else if (e.type == SDL_KEYUP) held_.erase(e.key.keysym.scancode);
}
bool Input::down(SDL_Scancode key) { return held_.count(key) != 0; }
bool Input::pressed(SDL_Scancode key) { return pressed_.count(key) != 0; }
void Input::endFrame() { pressed_.clear(); }
