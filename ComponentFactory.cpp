#include "ComponentFactory.h"
#include "Components.h"
#include "Engine.h"
#include <stdexcept>

ComponentFactory::ComponentFactory() {
    creators_["BodyComponent"]=[](GameObject& o,const YAML::Node& n){
        o.add<BodyComponent>(n["x"].as<float>(0),n["y"].as<float>(0),
            n["width"].as<float>(34),n["height"].as<float>(48),n["angle"].as<float>(0));
    };
    creators_["SpriteComponent"]=[](GameObject& o,const YAML::Node& n){
        SDL_Color c{Uint8(n["r"].as<int>(180)),Uint8(n["g"].as<int>(180)),Uint8(n["b"].as<int>(180)),255};
        o.add<SpriteComponent>(c,n["shape"].as<std::string>("rect"));
    };
    creators_["ControllerComponent"]=[](GameObject& o,const YAML::Node& n){
        o.add<ControllerComponent>(n["speed"].as<float>(260),n["jumpSpeed"].as<float>(520));
    };
    creators_["PatrolComponent"]=[](GameObject& o,const YAML::Node& n){
        o.add<PatrolComponent>(n["left"].as<float>(0),n["right"].as<float>(400),n["speed"].as<float>(90));
    };
    creators_["ChaseComponent"]=[](GameObject& o,const YAML::Node& n){
        o.add<ChaseComponent>(n["range"].as<float>(220),n["speed"].as<float>(115));
    };
    creators_["CombatComponent"]=[](GameObject& o,const YAML::Node& n){
        o.add<CombatComponent>(n["health"].as<int>(2),n["player"].as<bool>(false));
    };
}
std::unique_ptr<GameObject> ComponentFactory::create(const YAML::Node& node) const {
    if(!node.IsMap()) throw std::runtime_error("Each GameObject entry must be a YAML map.");
    auto object=std::make_unique<GameObject>();
    if(!node["components"]||!node["components"].IsSequence())
        throw std::runtime_error("GameObject needs a components sequence.");
    for(const auto& component:node["components"]){
        std::string type=component["type"].as<std::string>();
        auto it=creators_.find(type);
        if(it==creators_.end()) throw std::runtime_error("Unknown component type: "+type);
        it->second(*object,component);
    }
    return object;
}
std::vector<std::unique_ptr<GameObject>> ComponentFactory::load(const std::string& path) const {
    YAML::Node root=YAML::LoadFile(path);
    std::vector<std::unique_ptr<GameObject>> result;
    for(const auto& node:root["GameObjects"]) result.push_back(create(node));
    return result;
}
