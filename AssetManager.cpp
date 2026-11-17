#include "AssetManager.h"
#include <filesystem>
#include <iostream>

std::map<std::string, SDL_Texture*> AssetManager::textures_;

bool AssetManager::loadAssets(SDL_Renderer* renderer, const std::string& folder) {
    std::cout << "Current working directory: "
          << std::filesystem::current_path() << '\n';
    if (!std::filesystem::exists(folder)) {
        std::cerr << "Asset folder not found: " << folder << '\n';
        return false;
    }

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        std::cout << "Found file: " << entry.path().string() << '\n';
        if (!entry.is_regular_file() || entry.path().extension() != ".bmp") {
            continue;
        }

        SDL_Surface* surface = SDL_LoadBMP(entry.path().string().c_str());

        if (!surface) {
            std::cerr << "Failed to load: " << entry.path().string()
            << "\nSDL Error: " << SDL_GetError() << '\n';
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (texture) {
            std::string name = entry.path().stem().string();
            textures_[name] = texture;
            std::cout << "Loaded asset: " << name << '\n';
        }
    }

    return true;
}

SDL_Texture* AssetManager::get(const std::string& name) {
    auto it = textures_.find(name);
    return it != textures_.end() ? it->second : nullptr;
}

void AssetManager::clear() {
    for (auto& asset : textures_) {
        SDL_DestroyTexture(asset.second);
    }

    textures_.clear();
}