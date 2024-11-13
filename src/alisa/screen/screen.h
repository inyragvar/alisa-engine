#ifndef ALISA_SCREEN_SCREEN_H_
#define ALISA_SCREEN_SCREEN_H_

#include <vector>

#include "scene.h"

namespace alisa {
namespace screen {
    
class Screen {
public:
    Screen();
    virtual ~Screen();

    virtual bool init();
    virtual bool clear();

    virtual void draw();
    virtual void update(double dt);

    void addScene(Scene* scene);

private:
    std::vector<Scene*> scenes_;

    // debug flags
    bool show_poligon_line_mode_;
};

}
}  // namespace alisa

#endif