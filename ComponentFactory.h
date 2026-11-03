#pragma once
#include "GameObject.h"
#include <yaml-cpp/yaml.h>
#include <functional>
#include <map>
#include <memory>
#include <string>

class ComponentFactory {
public:
    using Creator=std::function<void(GameObject&,const YAML::Node&)>;
    ComponentFactory();
    std::unique_ptr<GameObject> create(const YAML::Node& node) const;
    std::vector<std::unique_ptr<GameObject>> load(const std::string& path) const;
private:
    std::map<std::string,Creator> creators_;
};
