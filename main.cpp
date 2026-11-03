#define SDL_MAIN_HANDLED
#include "Engine.h"
#include "ComponentFactory.h"
#include "Components.h"
#include <iostream>

int main(int argc,char** argv) {
    (void)argc; (void)argv;
    SDL_SetMainReady();
    if(!Engine::init("Escape Through Time - Component Demo",960,540)){
        std::cerr<<"SDL initialization failed: "<<SDL_GetError()<<"\n"; return 1;
    }
    try {
        ComponentFactory factory;
        auto objects=factory.load("assets/level1.yaml");
        for(auto& object:objects){
            if(auto* combat=object->get<CombatComponent>(); combat&&combat->player)
                Engine::setPlayer(object.get());
            Engine::add(std::move(object));
        }
        Engine::run();
    } catch(const std::exception& e) {
        std::cerr<<"Could not load level: "<<e.what()<<"\n";
        Engine::shutdown(); return 1;
    }
    Engine::shutdown();
    return 0;
}
