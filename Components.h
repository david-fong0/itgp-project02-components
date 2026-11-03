#pragma once
#include "Component.h"
#include <SDL.h>
#include <string>
#include <utility>

struct BodyComponent : Component {
    float x, y, vx{0}, vy{0}, angle{0}, w, h;
    bool grounded{false};
    BodyComponent(GameObject& p, float px, float py, float width=34, float height=48, float rotation=0)
        : Component(p), x(px), y(py), angle(rotation), w(width), h(height) {}
    void update() override;
    void draw() override {}
};

struct SpriteComponent : Component {
    SDL_Color color;
    std::string shape;
    SpriteComponent(GameObject& p, SDL_Color c, std::string s="rect")
        : Component(p), color(c), shape(std::move(s)) {}
    void update() override {}
    void draw() override;
};

struct ControllerComponent : Component {
    float speed, jumpSpeed;
    ControllerComponent(GameObject& p, float s=260, float j=520) : Component(p), speed(s), jumpSpeed(j) {}
    void update() override;
    void draw() override {}
};

struct PatrolComponent : Component {
    float left, right, speed;
    int direction{1};
    PatrolComponent(GameObject& p, float l, float r, float s=90)
        : Component(p), left(l), right(r), speed(s) {}
    void update() override;
    void draw() override {}
};

struct ChaseComponent : Component {
    float range, speed;
    ChaseComponent(GameObject& p, float r=220, float s=115) : Component(p), range(r), speed(s) {}
    void update() override;
    void draw() override {}
};

struct CombatComponent : Component {
    int health, maxHealth;
    float attackCooldown{0}, attackFlash{0};
    bool player;
    CombatComponent(GameObject& p, int hp=3, bool isPlayer=false)
        : Component(p), health(hp), maxHealth(hp), player(isPlayer) {}
    void update() override;
    void draw() override {}
    bool alive() const { return health > 0; }
    void damage(int amount=1) { health -= amount; }
};



// The two required behavior components, named for this game's enemy AI.
using Behavior1Component = PatrolComponent;
using Behavior2Component = ChaseComponent;
