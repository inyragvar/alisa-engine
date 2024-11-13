#include "entity_manager.h"

#include "alisa/log/log.h"

namespace alisa {
namespace entity {

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

void EntityManager::init() {
}

void EntityManager::clear() {
}

Sprite* EntityManager::getSprite(const std::string& name, const std::string& path, int frames_count, int frames_per_row) {
    system::StringID id = system::GetStringID(name);

    auto it = entities_.find(id);
    if (it != entities_.end()) {
        auto& counter = std::get<0>(it->second);
        counter++;

       // Log::info("EntityManager: entity: %s, counter: %d", std::get<1>(it->second)->getName(), counter);
        return dynamic_cast<Sprite*>(std::get<1>(it->second));
    }

    Sprite* sprite = new entity::Sprite(name, path, frames_count, frames_per_row);
    int counter = 1;

    auto value = std::make_tuple(counter, sprite);
    entities_[id] = value;
    
    return sprite;
}


void EntityManager::releaseEntity(const std::string& name) {
    if (name.empty()) {
        Log::warning("EntityManager: can't clear empty entity name");
    }
    system::StringID id = system::GetStringID(name);

    auto it = entities_.find(id);
    if (it != entities_.end()) {
        auto& counter = std::get<0>(it->second);
        counter--;

        if (counter == 0) {
            auto object = std::get<1>(it->second);
            entities_.erase(it);

            delete object;
        }
    }

    if (it == entities_.end()) {
        Log::info("EntityManager: can't find entity name: %s", name.c_str());
    } 
}

}
}