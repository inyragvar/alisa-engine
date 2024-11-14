#ifndef ALISA_RESOURCE_RESOURCE_MANAGER_H_
#define ALISA_RESOURCE_RESOURCE_MANAGER_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <tuple>
#include <utility>
#include <queue>

#include "texture.h"

#include "alisa/utils/singleton.h"

namespace alisa {
namespace resource {

class ResourceManager : public utils::Singleton<ResourceManager> {
    friend class Singleton<ResourceManager>;  // Allow Singleton to access private constructor

public:
    ~ResourceManager();

public:
    void init();
    void clear();

public:
    Texture* getTexture(const std::string& image_path, bool is_nearest = false);
    void releaseTexture(const std::string& image_path);

public:
    void initEmptyTexture();

private:
    ResourceManager();

protected:
    std::unordered_map<std::string, std::tuple<int, Texture*>> textures_;
};
}
}

#endif