#pragma once
#include "Component.h"
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

class GameObject {
public:
    template<class T, class... Args>
    T& add(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
        auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& result = *component;
        components_.push_back(std::move(component));
        return result;
    }

    template<class T> T* get() {
        for (auto& component : components_)
            if (auto* found = dynamic_cast<T*>(component.get())) return found;
        return nullptr;
    }
    template<class T> const T* get() const {
        for (const auto& component : components_)
            if (auto* found = dynamic_cast<const T*>(component.get())) return found;
        return nullptr;
    }

    void update() {
        // Components may add objects through the Engine, but should not mutate this list mid-update.
        for (auto& component : components_) component->update();
    }
    void draw() {
        for (auto& component : components_) component->draw();
    }
private:
    std::vector<std::unique_ptr<Component>> components_;
};
