#ifndef ALISA_INPUT_INPUT_MANAGER_H_
#define ALISA_INPUT_INPUT_MANAGER_H_

#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <map>
#include <mutex>
#include <thread>

#include "alisa/utils/singleton.h"

#include "event/key_event.h"
#include "event/mouse_event.h"
#include "event/mouse_move_event.h"
#include "event/quit_event.h"
#include "event/swipe_event.h"

#include "lib/SDL2/SDL.h"
#include "type.h"

#include "alisa/utils/math/vector_2f.h"

#include "alisa/utils/string_id.h"

namespace alisa {
namespace input {

class InputManager : public utils::Singleton<InputManager> {
    friend class utils::Singleton<InputManager>;

public:
    ~InputManager();

public:
    void init();
    void clear();

    bool catchEvents();

    void addQuitEvent(const quit_event_lambda lambda);
    void addKeyEvent(KeyMap key, const std::string& event_name, const key_event_lambda lambda);
    void addMouseEvent(MouseMap key, const std::string& event_name, const mouse_event_lambda lambda);
    void addMouseMoveEvent(const std::string& event_name, const mouse_move_event_lambda lambda);
    void addSwipeEvent(const std::string& event_name, const swipe_event_lambda lambda);

    void addContinuousKeyEvent(KeyMap key, const std::string& event_name, const key_event_lambda lambda);

public:
    void clearKeyEvent(KeyMap key, const std::string& event_name);
    void clearMouseEvent(MouseMap key, const std::string& event_name);
    void clearSwipeEvent(const std::string& event_name);

    void clearContinuousKeyEvent(KeyMap key, const std::string& event_name);

public:
    void lockEventHandlers();
    void unlockEventHandlers();

private:
    InputManager();

    void initConnectors();
    void initWindowRect();

    void handleEvents();

    void handleQuitEvent();

    void handleKeyEvent(const SDL_Event& event, bool is_pressed);
    void handleMouseMoveEvent(const SDL_Event& event);
    void handleMouseEvent(const SDL_Event& event, bool is_pressed);
    void handleTouchEvent(const SDL_Event& event, bool is_pressed);
    void handleSwipeEvent(float x, float y, bool is_pressed);

    void handleContinuousKeyEvent();

    bool hasKeyEvents() const;
    bool hasMouseMoveEvents() const;
    bool hasMouseEvents() const;
    bool hasTouchEvents() const;
    bool hasSwipeEvents() const;

private:
    bool is_running_;

    bool event_handlers_locked_;

    std::mutex event_mutex_;
    std::mutex key_event_mutex_;
    std::mutex mouse_event_mutex_;
    std::mutex swipe_event_mutex_;

    std::mutex event_handler_mutex_;

    std::thread* event_thread_;

    std::list<SDL_Event> event_queue_;

    std::map<SDL_Keycode, KeyMap> key_sdl_connector_;
   
    std::map<int, MouseMap> mouse_sdl_connector_;
    std::map<int, MouseMap> touch_sdl_connector_;

    std::map<KeyMap, unsigned int> continuous_key_sdl_connector_;

    // todo: clear events
    std::list<QuitEvent*> quit_events_;
    std::unordered_map<KeyMap, std::unordered_map<std::string, KeyEvent*>*> key_events_;
    std::unordered_map<MouseMap, std::unordered_map<std::string, MouseEvent*>*> mouse_events_;
    std::unordered_map<std::string, SwipeEvent*> swipe_events_;
    std::unordered_map<std::string, MouseMoveEvent*> mouse_move_events_;

    std::unordered_map<KeyMap, std::unordered_map<std::string, KeyEvent*>*> continuous_key_events_;

    utils::math::Vector2f aspect_ratio_;
    
    utils::math::Vector2f window_size_;
    utils::math::Vector2f screen_size_;

    utils::math::Vector2f swipe_start_position_;
};

}
}

#endif