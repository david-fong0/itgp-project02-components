#include "Engine.h"
#include "Input.h"
#include "Components.h"
#include <algorithm>
#include <cmath>

SDL_Window* Engine::window_=nullptr;
SDL_Renderer* Engine::renderer_=nullptr;
bool Engine::running_=false;
int Engine::width_=960, Engine::height_=540;
std::vector<std::unique_ptr<GameObject>> Engine::objects_;
GameObject* Engine::player_=nullptr;
std::string Engine::message_="Defeat the time-touched creatures to clear the beach route.";
float Engine::deltaTime_=0.016f;

bool Engine::init(const char* title, int w, int h) {
    width_=w; height_=h;
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) return false;
    window_=SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    if (!window_) return false;
    renderer_=SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) renderer_=SDL_CreateRenderer(window_, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer_) return false;
    running_=true; return true;
}
SDL_Renderer* Engine::renderer(){return renderer_;}
int Engine::width(){return width_;}
int Engine::height(){return height_;}
void Engine::add(std::unique_ptr<GameObject> o){objects_.push_back(std::move(o));}
std::vector<std::unique_ptr<GameObject>>& Engine::objects(){return objects_;}
GameObject* Engine::player(){return player_;}
void Engine::setPlayer(GameObject* p){player_=p;}
void Engine::setMessage(std::string m){message_=std::move(m); if(window_) SDL_SetWindowTitle(window_,message_.c_str());}
const std::string& Engine::message(){return message_;}
float Engine::deltaTime(){return deltaTime_;}

void Engine::drawWorld() {
    auto* r=renderer_;
    SDL_SetRenderDrawColor(r, 15, 25, 48, 255); SDL_RenderClear(r);
    // distant sky and ruins
    SDL_Rect moon{760,65,70,70}; SDL_SetRenderDrawColor(r,220,220,190,255); SDL_RenderFillRect(r,&moon);
    SDL_SetRenderDrawColor(r,27,43,67,255);
    for(int i=0;i<9;i++){ SDL_Rect ruin{ i*125-25, 245-(i%3)*24, 78, 170+(i%3)*24}; SDL_RenderFillRect(r,&ruin); }
    SDL_SetRenderDrawColor(r,49,65,79,255);
    SDL_Rect ground{0,470,width_,70}; SDL_RenderFillRect(r,&ground);
    SDL_SetRenderDrawColor(r,105,118,104,255);
    for(int i=0;i<width_;i+=48){ SDL_Rect stone{i,470,44,5}; SDL_RenderFillRect(r,&stone); }
    // platforms
    SDL_SetRenderDrawColor(r,75,86,93,255);
    SDL_Rect p1{210,370,155,18}, p2{475,315,145,18}, p3{700,390,150,18};
    SDL_RenderFillRect(r,&p1); SDL_RenderFillRect(r,&p2); SDL_RenderFillRect(r,&p3);
    for(auto& o:objects_) o->draw();
    // basic HUD drawn with bars (no font dependency)
    if(player_) if(auto* c=player_->get<CombatComponent>()){
        SDL_Rect panel{18,18,132,18}; SDL_SetRenderDrawColor(r,20,20,28,230); SDL_RenderFillRect(r,&panel);
        SDL_Rect hp{22,22,std::max(0,c->health)*40,10}; SDL_SetRenderDrawColor(r,205,75,83,255); SDL_RenderFillRect(r,&hp);
    }
    SDL_Rect instruction{18,50,290,5}; SDL_SetRenderDrawColor(r,170,184,192,255); SDL_RenderFillRect(r,&instruction);
    SDL_RenderPresent(r);
}
void Engine::removeDeadObjects(){
    objects_.erase(std::remove_if(objects_.begin(),objects_.end(),[](const auto& o){
        auto* c=o->template get<CombatComponent>(); return c && !c->alive() && !c->player;
    }),objects_.end());
    bool anyEnemy=false;
    for(const auto& o:objects_) {
        auto* c=o->get<CombatComponent>();
        if(c && !c->player && c->alive()) { anyEnemy=true; break; }
    }
    if(!anyEnemy) setMessage("Area clear! You defeated the time-touched creatures.");
}
void Engine::run(){
    Uint64 last=SDL_GetPerformanceCounter();
    while(running_){
        SDL_Event e; while(SDL_PollEvent(&e)){ if(e.type==SDL_QUIT) running_=false; Input::processEvent(e); }
        if(Input::pressed(SDL_SCANCODE_ESCAPE)) running_=false;
        Uint64 now=SDL_GetPerformanceCounter();
        float dt=std::min(0.033f,float(now-last)/SDL_GetPerformanceFrequency()); last=now;
        deltaTime_=dt;
        for(auto& o:objects_) o->update();
        removeDeadObjects();
        drawWorld();
        Input::endFrame();
    }
}
void Engine::shutdown(){
    objects_.clear(); player_=nullptr;
    if(renderer_) SDL_DestroyRenderer(renderer_);
    if(window_) SDL_DestroyWindow(window_);
    renderer_=nullptr; window_=nullptr; SDL_Quit();
}
