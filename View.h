#pragma once
#include <SDL.h>

class View {
public:
    View(int width, int height);

    void setCenter(float x, float y);
    void follow(float targetX, float targetY);

    SDL_Rect worldToScreen(float x, float y, int w, int h) const;

private:
    float centerX_;
    float centerY_;
    int width_;
    int height_;
};