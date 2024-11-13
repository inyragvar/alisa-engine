#include "screen_manager.h"

#include "alisa/log/log.h"
#include "alisa/opengl.h"
#include "alisa/render/render.h"
#include "alisa/input/input_manager.h"

#include <thread>
#include <chrono>

namespace alisa {
namespace screen {
ScreenManager* ScreenManager::instance_ = nullptr;
std::once_flag ScreenManager::init_instance_flag_;

void ScreenManager::initScreenManager() {
    instance_ = new ScreenManager();
}

ScreenManager& ScreenManager::get() {
    std::call_once(init_instance_flag_, &ScreenManager::initScreenManager);

    return *instance_;
}

ScreenManager::ScreenManager() : current_screen_(nullptr) {
    SDL_AtomicSet(&is_need_change_screen_, 0);
    SDL_AtomicSet(&is_need_reload_screen_, 0);
}

ScreenManager::~ScreenManager() {
    for (auto kv : screens_) {
        delete kv.second;
    }
}

bool ScreenManager::init(ScreenMap screens) {
    if (screens_.size() == 0) {
        for (auto kv : screens_) {
            kv.second->clear();
            delete kv.second;
        }
    }

    screens_ = screens;

    return true;
}

bool ScreenManager::clear() {
    for (auto kv : screens_) {
        kv.second->clear();
    }

    return true;
}

Screen* ScreenManager::getCurrent() { 
    return current_screen_; 
}

bool ScreenManager::setCurrent(std::string name) {
    if (screens_.find(name) == screens_.end()) {
        return false;
    }

    if (current_screen_ == nullptr) {
        current_screen_ = screens_[name];
        current_screen_name_ = name;
        current_screen_->init();
        return true;
    }

    input::InputManager::get().lockEventHandlers();
    new_screen_name_ = name;
    SDL_AtomicSet(&is_need_change_screen_, 1);
    return true;
}

void ScreenManager::changeScreen() {
    if (SDL_AtomicGet(&is_need_change_screen_) == 1) {
        Screen* new_screen = screens_[new_screen_name_];
        
        Screen* prev_screen = current_screen_;
        prev_screen->clear();

        new_screen->init();
        
        current_screen_ = new_screen;
        current_screen_name_ = new_screen_name_;
        
        SDL_AtomicSet(&is_need_change_screen_, 0);
        input::InputManager::get().unlockEventHandlers();
        std::this_thread::sleep_for(std::chrono::duration<double>(0.05));
    }
}

void ScreenManager::reloadCurrentScreen() {
    setCurrent(current_screen_name_);
}

void ScreenManager::addScreen(std::string name, Screen* screen) {
    screens_[name] = screen;
}
}
}  // namespace alisa
