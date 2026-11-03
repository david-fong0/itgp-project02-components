#pragma once
class GameObject;

class Component {
public:
    explicit Component(GameObject& owner) : owner_(owner) {}
    virtual ~Component() = default;
    virtual void update() = 0;
    virtual void draw() = 0;
    GameObject& parent() { return owner_; }
    const GameObject& parent() const { return owner_; }
private:
    GameObject& owner_;
};
