#include "Components.h"
#include "GameObject.h"
#include "Engine.h"
#include "Input.h"
#include <algorithm>
#include <cmath>

static SDL_Rect rectFor(const BodyComponent& b) {
    return {int(b.x),int(b.y),int(b.w),int(b.h)};
}
void BodyComponent::update() {
    const float dt=Engine::deltaTime();
    // Lightweight platformer physics: gravity, screen bounds, and three ledges.
    vy += 1250.0f * dt;
    x += vx * dt;
    y += vy * dt;
    x = std::clamp(x, 0.0f, float(Engine::width()) - w);
    grounded = false;
    struct Platform { float x,y,w; };
    const Platform platforms[]={{0,470,float(Engine::width())},{210,370,155},{475,315,145},{700,390,150}};
    for (const auto& p : platforms) {
        bool overlaps = x + w > p.x && x < p.x + p.w;
        if (overlaps && vy >= 0 && y + h >= p.y && y + h - vy * dt <= p.y + 2) {
            y = p.y - h; vy = 0; grounded = true;
        }
    }
    if (y > Engine::height() + 100) { y=420; vy=0; }
}

void SpriteComponent::draw() {
    auto* body=parent().get<BodyComponent>(); if(!body) return;
    SDL_Rect dst=rectFor(*body);
    SDL_SetRenderDrawColor(Engine::renderer(),color.r,color.g,color.b,255);
    SDL_RenderFillRect(Engine::renderer(),&dst);
    // simple face/armor detail gives the primitive sprites character
    SDL_SetRenderDrawColor(Engine::renderer(),20,25,34,255);
    SDL_Rect eye{dst.x+(dst.w/2),dst.y+10,5,5}; SDL_RenderFillRect(Engine::renderer(),&eye);
    if(shape=="player"){
        SDL_SetRenderDrawColor(Engine::renderer(),205,210,218,255);
        SDL_Rect blade{dst.x+dst.w-2,dst.y+20,18,5}; SDL_RenderFillRect(Engine::renderer(),&blade);
        if (auto* combat=parent().get<CombatComponent>(); combat && combat->attackFlash>0) {
            SDL_SetRenderDrawColor(Engine::renderer(),245,235,160,255);
            SDL_Rect slash{dst.x+dst.w+(body->angle==0?0:-48),dst.y+8,48,30};
            SDL_RenderDrawRect(Engine::renderer(),&slash);
        }
    }
}
void ControllerComponent::update() {
    const float dt=Engine::deltaTime();
    auto* b=parent().get<BodyComponent>(); if(!b) return;
    if (auto* c=parent().get<CombatComponent>(); c && c->health<=0) { b->vx=0; return; }
    float axis=0;
    if(Input::down(SDL_SCANCODE_A)||Input::down(SDL_SCANCODE_LEFT)) axis-=1;
    if(Input::down(SDL_SCANCODE_D)||Input::down(SDL_SCANCODE_RIGHT)) axis+=1;
    b->vx=axis*speed;
    if(axis!=0) b->angle=axis>0?0:180;
    if((Input::pressed(SDL_SCANCODE_SPACE)||Input::pressed(SDL_SCANCODE_W)||Input::pressed(SDL_SCANCODE_UP)) && b->grounded){
        b->vy=-jumpSpeed; b->grounded=false;
    }
    if(Input::pressed(SDL_SCANCODE_J) && Engine::player()==&parent()){
        auto* combat=parent().get<CombatComponent>();
        if(combat && combat->attackCooldown<=0){
            combat->attackCooldown=0.32f; combat->attackFlash=0.13f;
            // The attack is resolved immediately against nearby enemies, including while moving.
            for(auto& object:Engine::objects()){
                if(object.get()==&parent()) continue;
                auto* enemyBody=object->get<BodyComponent>(); auto* enemy=object->get<CombatComponent>();
                if(!enemyBody||!enemy||enemy->player||!enemy->alive()) continue;
                float dx=(enemyBody->x+enemyBody->w/2)-(b->x+b->w/2);
                float dy=std::abs((enemyBody->y+enemyBody->h/2)-(b->y+b->h/2));
                bool facing=(b->angle==0?dx>=-5:dx<=5);
                if(facing && std::abs(dx)<78 && dy<55) enemy->damage();
            }
        }
    }
}
void PatrolComponent::update() {
    auto* b=parent().get<BodyComponent>(); if(!b) return;
    if(b->x<left){b->x=left;direction=1;}
    if(b->x+b->w>right){b->x=right-b->w;direction=-1;}
    b->vx=direction*speed;
}
void ChaseComponent::update() {
    auto* b=parent().get<BodyComponent>(); auto* p=Engine::player()?Engine::player()->get<BodyComponent>():nullptr;
    if(!b||!p) return;
    float dx=(p->x+p->w/2)-(b->x+b->w/2);
    b->vx=0;
    if(std::abs(dx)<range && std::abs(p->y-b->y)<100){
        b->vx=(dx>0?1.0f:-1.0f)*speed; b->angle=dx>0?0:180;
    }
}
void CombatComponent::update() {
    const float dt=Engine::deltaTime();
    attackCooldown=std::max(0.0f,attackCooldown-dt);
    attackFlash=std::max(0.0f,attackFlash-dt);
    auto* b=parent().get<BodyComponent>(); if(!b||player||!alive()) return;
    auto* p=Engine::player(); if(!p) return;
    auto* pb=p->get<BodyComponent>(); auto* pc=p->get<CombatComponent>();
    if(pb&&pc&&std::abs((pb->x+pb->w/2)-(b->x+b->w/2))<32&&std::abs(pb->y-b->y)<48&&attackCooldown<=0){
        pc->damage(); attackCooldown=0.9f;
        if(pc->health<=0) Engine::setMessage("The knight has fallen. Close the game and run again to retry.");
    }
    if(health<=0) Engine::setMessage("Enemy defeated! Keep moving toward the relic.");
}
