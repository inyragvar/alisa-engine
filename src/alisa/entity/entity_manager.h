#ifndef ALISA_ENTITY_ENTITY_MANAGER_H_
#define ALISA_ENTITY_ENTITY_MANAGER_H_

#include <string>
#include <unordered_map>
#include <tuple>
#include <utility>

#include "alisa/utils/singleton.h"
#include "alisa/utils/string_id.h"

#include "sprite.h"

namespace alisa {
namespace entity {

class EntityManager : public utils::Singleton<EntityManager> {
    friend class Singleton<EntityManager>;

public:
    ~EntityManager();

public:
    void init();
    void clear();

public:
    Sprite* getSprite(const std::string& name, const std::string& path, int frames_count = 1, int frames_per_row = 1);

public:
    void releaseEntity(const std::string& name);

protected:
    EntityManager();

protected:
    std::unordered_map<utils::StringID, std::tuple<int, Entity*>> entities_;
};

}
}

#endif