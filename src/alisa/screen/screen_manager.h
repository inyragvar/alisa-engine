#ifndef ALISA_SCREEN_SCREEN_MANAGER_H_
#define ALISA_SCREEN_SCREEN_MANAGER_H_

#include <unordered_map>
#include <string>
#include <mutex>

#include "screen.h"

#include "alisa/utils/singleton.h"
#include "lib/SDL2/SDL.h"

namespace alisa {
namespace screen {

typedef std::unordered_map<std::string, Screen*> ScreenMap;

class ScreenManager : public utils::Singleton<ScreenManager> {
    friend class Singleton<ScreenManager>;

public:
    virtual ~ScreenManager();

    virtual bool init(ScreenMap screens);
    virtual bool clear();

    Screen* getCurrent();
    bool setCurrent(std::string name);

    void changeScreen();

    void reloadCurrentScreen();

protected:
    ScreenManager();
    void addScreen(std::string name, Screen* screen);

private:
    static void initScreenManager();

private:
    Screen* current_screen_;

    ScreenMap screens_;

    SDL_atomic_t is_need_change_screen_;
    SDL_atomic_t is_need_reload_screen_;
    std::string current_screen_name_;
    std::string new_screen_name_;
};

}
} 

#endif