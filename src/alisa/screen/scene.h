#ifndef ALISA_SCREEN_SCENE_H_
#define ALISA_SCREEN_SCENE_H_

#include <string>
#include <unordered_map>
#include <map>

#include "alisa/utils/math/vector_3f.h"

namespace alisa {
namespace screen {

class Scene {
public:
    virtual ~Scene();

public:
    virtual bool init() = 0;
    virtual bool clear() = 0;

public:
    virtual void draw() = 0;
    virtual void update(double dt) = 0;
};

}
}  // namespace alisa

#endif