#include "View.h"

View::View(int width, int height)
    : centerX_(width / 2.0f),
      centerY_(height / 2.0f),
      width_(width),
      height_(height) {
}

void View::setCenter(float x, float y) {
    centerX_ = x;
    centerY_ = y;
}

void View::follow(float targetX, float targetY) {
    centerX_ = targetX;
    centerY_ = targetY;
}

SDL_Rect View::worldToScreen(float x, float y, int w, int h) const {
    return {
        static_cast<int>(x - centerX_ + width_ / 2.0f),
        static_cast<int>(y - centerY_ + height_ / 2.0f),
        w,
        h
    };
}