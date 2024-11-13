#include "screen.h"

namespace alisa {
namespace screen {

Screen::Screen() {}

Screen::~Screen() {}

bool Screen::init(){
    for (Scene* scene : scenes_) {
        scene->init();
    }

    return true;
}

bool Screen::clear() {
    for (Scene* scene : scenes_) {
        scene->clear();
    }
    return true;
}

void Screen::draw() {
    for (Scene* scene : scenes_) {
        scene->draw();
    }
}

void Screen::update(double dt) {
    for (Scene* scene : scenes_) {
        scene->update(dt);
    }
}

void Screen::addScene(Scene* scene) {
    scenes_.push_back(scene);
}

}
}  // namespace alisa