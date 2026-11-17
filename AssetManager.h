#pragma once

#include <SDL.h>
#include <map>
#include <string>

class AssetManager {
public:
    static bool loadAssets(SDL_Renderer* renderer, const std::string& folder);
    static SDL_Texture* get(const std::string& name);
    static void clear();

private:
    static std::map<std::string, SDL_Texture*> textures_;
};