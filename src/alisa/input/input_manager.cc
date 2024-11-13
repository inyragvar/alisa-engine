#include "input_manager.h"

#include <cstdlib>

#include "alisa/logger/logger.h"
#include "alisa/utils/config.h"
#include "alisa/screen/screen_manager.h"

#if defined(__IPHONEOS__) || defined(__EMSCRIPTEN__)
#include "alisa/render/render.h"
#endif 

namespace alisa {
namespace input {

InputManager::InputManager(): event_handlers_locked_(false) {}

InputManager::~InputManager() {}

void InputManager::init() {
    initWindowRect();

    logger::Logger::info("InputManager: window size: %f x %f; screen size: %f x %f; aspect ration: %f x %f", 
              window_size_.x, window_size_.y, screen_size_.x, screen_size_.y, aspect_ratio_.x, aspect_ratio_.y);

    is_running_ = true;

    initConnectors();
}

void InputManager::clear() {
}

bool InputManager::catchEvents() {
    SDL_Event event;
    
    int new_width;
    int new_height;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                handleKeyEvent(event, true);
                break;
            case SDL_KEYUP:
                handleKeyEvent(event, false);
                break;
            case SDL_MOUSEMOTION:
                handleMouseMoveEvent(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
            #ifndef MOBILE_OS
                handleMouseEvent(event, true);
            #endif
                break;
            case SDL_MOUSEBUTTONUP:
            #ifndef MOBILE_OS
                handleMouseEvent(event, false);
            #endif
                break;
            case SDL_FINGERDOWN:
                handleTouchEvent(event, true);
                break;
            case SDL_FINGERUP:
                handleTouchEvent(event, false);
                break;
           case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        new_width = event.window.data1;
                        new_height = event.window.data2;
                        
                    #if defined(__IPHONEOS__) || defined(__EMSCRIPTEN__)
                        Render::get().getWindowSize(&new_width, &new_height);
                    #endif

                        //utils::Config::get().set("window_width", new_width);
                       // utils::Config::get().set("window_height", new_height);

                       // system::AdaptScreenSize();
                        screen::ScreenManager::get().reloadCurrentScreen();
                        initWindowRect();

                        logger::Logger::info("Window size changed: w=%d, h=%d\n", new_width, new_height);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        //	game_->setActive(false);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        //	game_->setActive(true);
                        break;
                }
                break;
            //#ifndef MOBILE_OS
            case SDL_SENSORUPDATE:
                switch (event.sensor.type) {
                    case SDL_SENSOR_ACCEL:
                        float x = event.sensor.data[0];
                        float y = event.sensor.data[1];
                        float z = event.sensor.data[2];
                        // Handle accelerometer sensor data
                        logger::Logger::info("Accelerometer: x=%.2f, y=%.2f, z=%.2f\n", x, y, z);
                    break;
                }
           // #endif
            case SDL_QUIT:
                handleQuitEvent();
                break;
        }
    }

    handleContinuousKeyEvent();
    
    return true;
}

void InputManager::addQuitEvent(const quit_event_lambda lambda) {
    quit_events_.push_back(new QuitEvent(lambda));
}

void InputManager::addKeyEvent(KeyMap key, const std::string& event_name, const key_event_lambda lambda) {
    const auto event = new KeyEvent(lambda);

    auto key_event = key_events_.find(key);
    if (key_event != key_events_.end()) {
        auto event_map = key_event->second;

        auto it = event_map->find(event_name);
        if (it != event_map->end()) {
          //  std::lock_guard<std::mutex> lock(key_event_mutex_);
            delete it->second;
            event_map->erase(it); 
            logger::Logger::info("InputManager: clearing event duplication for key: %d and event_name: %s", key, event_name.c_str());
        }
        (*event_map)[event_name] = event;
    } else {
        auto event_map = new std::unordered_map<std::string, KeyEvent*>();
        (*event_map)[event_name] = event;

        key_events_[key] = event_map;
    }
}

void InputManager::addMouseEvent(MouseMap key, const std::string& event_name, const mouse_event_lambda lambda) {
    const auto event = new MouseEvent(lambda);

    auto mouse_event = mouse_events_.find(key);
    if (mouse_event != mouse_events_.end()) {
        auto event_map = mouse_event->second;

        auto it = event_map->find(event_name);
        if (it != event_map->end()) {
           // std::lock_guard<std::mutex> lock(mouse_event_mutex_);
            delete it->second;
            event_map->erase(it); 
            logger::Logger::warn("InputManager: clearing event duplication for mouse: %d and event_name: %s", key, event_name.c_str());
        }

        (*event_map)[event_name] = event;
    } else {
        auto event_map = new std::unordered_map<std::string, MouseEvent*>();
        (*event_map)[event_name] = event;

        mouse_events_[key] = event_map;
    }
}

void InputManager::addMouseMoveEvent(const std::string& event_name, const mouse_move_event_lambda lambda) {
    const auto event = new MouseMoveEvent(lambda);

    auto it = mouse_move_events_.find(event_name);
    if (it != mouse_move_events_.end()) {
        // std::lock_guard<std::mutex> lock(mouse_event_mutex_);
        delete it->second;
        mouse_move_events_.erase(it); 
        logger::Logger::warn("InputManager: clearing event duplication for mouse move event_name: %s", event_name.c_str());
    }

    mouse_move_events_[event_name] = event;
}

 void InputManager::addSwipeEvent(const std::string& event_name, const swipe_event_lambda lambda) {
    const auto event = new SwipeEvent(lambda);

    auto it = swipe_events_.find(event_name);
    if (it != swipe_events_.end()) {
      //  std::lock_guard<std::mutex> lock(swipe_event_mutex_);
        delete it->second;
        swipe_events_.erase(it); 
        logger::Logger::warn("InputManager: clearing swipe event duplication for event_name: %s", event_name.c_str());
    }

    swipe_events_[event_name] = event;
}


void InputManager::addContinuousKeyEvent(KeyMap key, const std::string& event_name, const key_event_lambda lambda) {
    const auto event = new KeyEvent(lambda);

    auto key_event = continuous_key_events_.find(key);
    if (key_event != continuous_key_events_.end()) {
        auto event_map = key_event->second;

        auto it = event_map->find(event_name);
        if (it != event_map->end()) {
            delete it->second;
            event_map->erase(it); 
            logger::Logger::info("InputManager: clearing event duplication for continuous key: %d and event_name: %s", key, event_name.c_str());
        }
        (*event_map)[event_name] = event;
    } else {
        auto event_map = new std::unordered_map<std::string, KeyEvent*>();
        (*event_map)[event_name] = event;

        continuous_key_events_[key] = event_map;
    }
}

void InputManager::clearKeyEvent(KeyMap key, const std::string& event_name) {
    auto key_event = key_events_.find(key);
    if (key_event != key_events_.end()) {
       // std::lock_guard<std::mutex> lock(key_event_mutex_);
        auto event_map = key_event->second;

        auto it = event_map->find(event_name);
        if (it != event_map->end()) {
            delete it->second; 
            event_map->erase(it); 
            logger::Logger::info("InputManager: clearing event for key: %d and event_name: %s", key, event_name.c_str());
        }
    }
}

void InputManager::clearMouseEvent(MouseMap key, const std::string& event_name) {
    auto mouse_event = mouse_events_.find(key);
    if (mouse_event != mouse_events_.end()) {
        //std::lock_guard<std::mutex> lock(mouse_event_mutex_);
        auto event_map = mouse_event->second;

        auto it = event_map->find(event_name);
        if (it != event_map->end()) {
            delete it->second; 
            event_map->erase(it); 
            logger::Logger::info("InputManager: clearing event for mouse: %d and event_name: %s", key, event_name.c_str());
        }
    }
}

void InputManager::clearSwipeEvent(const std::string& event_name) {
   // std::lock_guard<std::mutex> lock(swipe_event_mutex_);

    auto it = swipe_events_.find(event_name);
    if (it != swipe_events_.end()) {
        delete it->second; 
        swipe_events_.erase(it); 
        logger::Logger::info("InputManager: clearing swipe event for event_name: %s", event_name.c_str());
    }
}

void InputManager::clearContinuousKeyEvent(KeyMap key, const std::string& event_name) {
    auto key_event = continuous_key_events_.find(key);
    if (key_event != continuous_key_events_.end()) {
        auto event_map = key_event->second;

        auto it = event_map->find(event_name);
        if (it != event_map->end()) {
            delete it->second; 
            event_map->erase(it); 
            logger::Logger::info("InputManager: clearing event for continuous key: %d and event_name: %s", key, event_name.c_str());
        }
    }
}

void InputManager::lockEventHandlers() {
    if (event_handlers_locked_) {
        return;
    }
    
   // event_handler_mutex_.lock();
    event_handlers_locked_ = true;

}
void InputManager::unlockEventHandlers() {
    event_handlers_locked_ = false;
   // SDL_Delay(1000);
    //event_handler_mutex_.unlock();
}

void InputManager::initConnectors() {
    key_sdl_connector_[SDLK_q] = KeyMap::q;
    key_sdl_connector_[SDLK_w] = KeyMap::w;
    key_sdl_connector_[SDLK_e] = KeyMap::e;
    key_sdl_connector_[SDLK_r] = KeyMap::r;
    key_sdl_connector_[SDLK_t] = KeyMap::t;
    key_sdl_connector_[SDLK_y] = KeyMap::y;
    key_sdl_connector_[SDLK_u] = KeyMap::u;
    key_sdl_connector_[SDLK_i] = KeyMap::i;
    key_sdl_connector_[SDLK_o] = KeyMap::o;
    key_sdl_connector_[SDLK_p] = KeyMap::p;
    key_sdl_connector_[SDLK_a] = KeyMap::a;
    key_sdl_connector_[SDLK_s] = KeyMap::s;
    key_sdl_connector_[SDLK_d] = KeyMap::d;
    key_sdl_connector_[SDLK_f] = KeyMap::f;
    key_sdl_connector_[SDLK_g] = KeyMap::g;
    key_sdl_connector_[SDLK_h] = KeyMap::h;
    key_sdl_connector_[SDLK_j] = KeyMap::j;
    key_sdl_connector_[SDLK_k] = KeyMap::k;
    key_sdl_connector_[SDLK_l] = KeyMap::l;
    key_sdl_connector_[SDLK_z] = KeyMap::z;
    key_sdl_connector_[SDLK_x] = KeyMap::x;
    key_sdl_connector_[SDLK_c] = KeyMap::c;
    key_sdl_connector_[SDLK_v] = KeyMap::v;
    key_sdl_connector_[SDLK_b] = KeyMap::b;
    key_sdl_connector_[SDLK_n] = KeyMap::n;
    key_sdl_connector_[SDLK_m] = KeyMap::m;

    key_sdl_connector_[SDLK_UP] = KeyMap::up;
    key_sdl_connector_[SDLK_DOWN] = KeyMap::down;
    key_sdl_connector_[SDLK_LEFT] = KeyMap::left;
    key_sdl_connector_[SDLK_RIGHT] = KeyMap::right;

    key_sdl_connector_[SDLK_SPACE] = KeyMap::space;
    key_sdl_connector_[SDLK_BACKSPACE] = KeyMap::backspace;

    key_sdl_connector_[SDLK_0] = KeyMap::num_0;
    key_sdl_connector_[SDLK_1] = KeyMap::num_1;
    key_sdl_connector_[SDLK_2] = KeyMap::num_2;
    key_sdl_connector_[SDLK_3] = KeyMap::num_3;
    key_sdl_connector_[SDLK_4] = KeyMap::num_4;
    key_sdl_connector_[SDLK_5] = KeyMap::num_5;
    key_sdl_connector_[SDLK_6] = KeyMap::num_6;
    key_sdl_connector_[SDLK_7] = KeyMap::num_7;
    key_sdl_connector_[SDLK_8] = KeyMap::num_8;
    key_sdl_connector_[SDLK_9] = KeyMap::num_9;

    mouse_sdl_connector_[SDL_BUTTON_LEFT] = MouseMap::left;
    mouse_sdl_connector_[SDL_BUTTON_RIGHT] = MouseMap::right;

    touch_sdl_connector_[SDL_BUTTON_LEFT] = MouseMap::left;

    continuous_key_sdl_connector_[KeyMap::a] = SDL_SCANCODE_A;
    continuous_key_sdl_connector_[KeyMap::b] = SDL_SCANCODE_B;
    continuous_key_sdl_connector_[KeyMap::c] = SDL_SCANCODE_C;
    continuous_key_sdl_connector_[KeyMap::d] = SDL_SCANCODE_D;
    continuous_key_sdl_connector_[KeyMap::e] = SDL_SCANCODE_E;
    continuous_key_sdl_connector_[KeyMap::f] = SDL_SCANCODE_F;
    continuous_key_sdl_connector_[KeyMap::g] = SDL_SCANCODE_G;
    continuous_key_sdl_connector_[KeyMap::h] = SDL_SCANCODE_H;
    continuous_key_sdl_connector_[KeyMap::i] = SDL_SCANCODE_I;
    continuous_key_sdl_connector_[KeyMap::j] = SDL_SCANCODE_J;
    continuous_key_sdl_connector_[KeyMap::k] = SDL_SCANCODE_K;
    continuous_key_sdl_connector_[KeyMap::l] = SDL_SCANCODE_L;
    continuous_key_sdl_connector_[KeyMap::m] = SDL_SCANCODE_M;
    continuous_key_sdl_connector_[KeyMap::n] = SDL_SCANCODE_N;
    continuous_key_sdl_connector_[KeyMap::o] = SDL_SCANCODE_O;
    continuous_key_sdl_connector_[KeyMap::p] = SDL_SCANCODE_P;
    continuous_key_sdl_connector_[KeyMap::q] = SDL_SCANCODE_Q;
    continuous_key_sdl_connector_[KeyMap::r] = SDL_SCANCODE_R;
    continuous_key_sdl_connector_[KeyMap::s] = SDL_SCANCODE_S;
    continuous_key_sdl_connector_[KeyMap::t] = SDL_SCANCODE_T;
    continuous_key_sdl_connector_[KeyMap::u] = SDL_SCANCODE_U;
    continuous_key_sdl_connector_[KeyMap::v] = SDL_SCANCODE_V;
    continuous_key_sdl_connector_[KeyMap::w] = SDL_SCANCODE_W;
    continuous_key_sdl_connector_[KeyMap::x] = SDL_SCANCODE_X;
    continuous_key_sdl_connector_[KeyMap::y] = SDL_SCANCODE_Y;
    continuous_key_sdl_connector_[KeyMap::z] = SDL_SCANCODE_Z;

    continuous_key_sdl_connector_[KeyMap::num_1] = SDL_SCANCODE_1;
    continuous_key_sdl_connector_[KeyMap::num_2] = SDL_SCANCODE_2;
    continuous_key_sdl_connector_[KeyMap::num_3] = SDL_SCANCODE_3;
    continuous_key_sdl_connector_[KeyMap::num_4] = SDL_SCANCODE_4;
    continuous_key_sdl_connector_[KeyMap::num_5] = SDL_SCANCODE_5;
    continuous_key_sdl_connector_[KeyMap::num_6] = SDL_SCANCODE_6;
    continuous_key_sdl_connector_[KeyMap::num_7] = SDL_SCANCODE_7;
    continuous_key_sdl_connector_[KeyMap::num_8] = SDL_SCANCODE_8;
    continuous_key_sdl_connector_[KeyMap::num_9] = SDL_SCANCODE_9;
    continuous_key_sdl_connector_[KeyMap::num_0] = SDL_SCANCODE_0;

    continuous_key_sdl_connector_[KeyMap::right] = SDL_SCANCODE_RIGHT;
    continuous_key_sdl_connector_[KeyMap::left] = SDL_SCANCODE_LEFT;
    continuous_key_sdl_connector_[KeyMap::down] = SDL_SCANCODE_DOWN;
    continuous_key_sdl_connector_[KeyMap::up] = SDL_SCANCODE_UP;

    continuous_key_sdl_connector_[KeyMap::space] = SDL_SCANCODE_SPACE;
    continuous_key_sdl_connector_[KeyMap::backspace] = SDL_SCANCODE_BACKSPACE;
}

void InputManager::initWindowRect() {
    window_size_.x = utils::Config::get().getInt("window_width");
    window_size_.y = utils::Config::get().getInt("window_height");

    screen_size_.x = utils::Config::get().getInt("adapted_screen_width");
    screen_size_.y = utils::Config::get().getInt("adapted_screen_height");

    aspect_ratio_.x = screen_size_.x / window_size_.x;
    aspect_ratio_.y = screen_size_.y / window_size_.y;
}

void InputManager::handleEvents() {
    int new_width;
    int new_height;

   while (is_running_) {
        if (event_queue_.empty()) {
            continue;
        }
       
        event_mutex_.lock();
        const SDL_Event event = event_queue_.front();
        event_queue_.pop_front();
        event_mutex_.unlock();    

        switch (event.type) {
            case SDL_KEYDOWN:
                handleKeyEvent(event, true);
                break;
            case SDL_KEYUP:
                handleKeyEvent(event, false);
                break;
            case SDL_MOUSEMOTION:
                handleMouseMoveEvent(event);
                break;
            case SDL_MOUSEBUTTONDOWN:
            #ifndef MOBILE_OS
                handleMouseEvent(event, true);
            #endif
                break;
            case SDL_MOUSEBUTTONUP:
            #ifndef MOBILE_OS
                handleMouseEvent(event, false);
            #endif
                break;
            case SDL_FINGERDOWN:
                handleTouchEvent(event, true);
                break;
            case SDL_FINGERUP:
                handleTouchEvent(event, false);
                break;
           case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        new_width = event.window.data1;
                        new_height = event.window.data2;
                        
                    #if defined(__IPHONEOS__) || defined(__EMSCRIPTEN__)
                        Render::get().getWindowSize(&new_width, &new_height);
                    #endif

                        //utils::Config::get().set("window_width", new_width);
                       // utils::Config::get().set("window_height", new_height);

                       // system::AdaptScreenSize();
                        screen::ScreenManager::get().reloadCurrentScreen();
                        initWindowRect();

                        logger::Logger::info("Window size changed: w=%d, h=%d\n", new_width, new_height);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        //	game_->setActive(false);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        //	game_->setActive(true);
                        break;
                }
                break;
            //#ifndef MOBILE_OS
            case SDL_SENSORUPDATE:
                switch (event.sensor.type) {
                    case SDL_SENSOR_ACCEL:
                        float x = event.sensor.data[0];
                        float y = event.sensor.data[1];
                        float z = event.sensor.data[2];
                        // Handle accelerometer sensor data
                        logger::Logger::info("Accelerometer: x=%.2f, y=%.2f, z=%.2f\n", x, y, z);
                    break;
                }
           // #endif
            case SDL_QUIT:
                handleQuitEvent();
                break;
        }
    }
}

void InputManager::handleQuitEvent() {
    for (QuitEvent* event : quit_events_) {
        event->call();
    }
}

void InputManager::handleKeyEvent(const SDL_Event& event, bool is_pressed) {
    if (event_handlers_locked_) {
        logger::Logger::info("InputManager: skipping key event: %d", event.key.keysym.sym);
        return;
    }

    if (key_events_.size() == 0) {
        return;
    }

    int key_id = event.key.keysym.sym;
    logger::Logger::info("Key pressed: %d; is pressed: %s", key_id, is_pressed ? "true" : "false");

    if (key_sdl_connector_.count(key_id) > 0) {
        const KeyMap key = key_sdl_connector_[key_id];

        const KeyData data = KeyData{is_pressed};

        if (key_events_.count(key) > 0) {
          //  std::lock_guard<std::mutex> lock(key_event_mutex_);
            auto key_map = key_events_[key];
            for (auto value : *key_map) {
                value.second->call(data);
            }
        }
    }
}

void InputManager::handleMouseMoveEvent(const SDL_Event& event) {
    if (event_handlers_locked_) {
        logger::Logger::info("InputManager: skipping mouse event: %d", event.key.keysym.sym);
        return;
    }

    if (mouse_move_events_.size() == 0) {
        return;
    }

    float x = event.motion.x * aspect_ratio_.x;
    float y = (window_size_.y - event.motion.y) * aspect_ratio_.y;

    const MouseMoveData data = MouseMoveData{x, y, event.motion.xrel * 1.0f, event.motion.yrel * 1.0f, false};

    for (auto value : mouse_move_events_) {
        value.second->call(data);
    }
}

void InputManager::handleMouseEvent(const SDL_Event& event, bool is_pressed) {
    if (event_handlers_locked_) {
        logger::Logger::info("InputManager: skipping mouse event: %d", event.key.keysym.sym);
        return;
    }

    if (mouse_events_.size() == 0) {
        return;
    }

    int key_id = event.button.button;
    logger::Logger::info("Mouse button pressed: %d; is pressed: %s", key_id, is_pressed ? "true" : "false");

    if (mouse_sdl_connector_.count(key_id) > 0) {
        MouseMap key = mouse_sdl_connector_[key_id];

        float x = event.button.x * aspect_ratio_.x;
        float y = (window_size_.y - event.button.y) * aspect_ratio_.y;

        if (key_id == SDL_BUTTON_LEFT) {
           handleSwipeEvent(x, y, is_pressed);     
        }

        const MouseData data = MouseData{x, y, is_pressed};

        if (mouse_events_.count(key) > 0) {
           // std::lock_guard<std::mutex> lock(mouse_event_mutex_);
            auto mouse_map = mouse_events_[key];
            for (auto value : *mouse_map) {
                value.second->call(data);
            }
        }
    }   
}

void InputManager::handleTouchEvent(const SDL_Event& event, bool is_pressed) {
    if (event_handlers_locked_) {
        logger::Logger::info("InputManager: skipping touch event: %d", event.key.keysym.sym);
        return;
    }
    MouseMap key = mouse_sdl_connector_[SDL_BUTTON_LEFT];

    if (mouse_events_.size() == 0 && swipe_events_.size() == 0) {
        return;
    }

    // todo: add transformation for coordinates
    float x = event.tfinger.x * screen_size_.x;
    float y = (1.0 - event.tfinger.y) * screen_size_.y;

    logger::Logger::info("touched x: %f, y: %f; is pressed: %s", x, y,
              is_pressed ? "true" : "false");

    handleSwipeEvent(x, y, is_pressed);

    const MouseData data = MouseData{x, y, is_pressed};

    if (mouse_events_.count(key) > 0) {
        auto touch_map = mouse_events_[key];
        for (auto value : *touch_map) {
            value.second->call(data);
        }
    }
}

void InputManager::handleSwipeEvent(float x, float y, bool is_pressed) {
    if (is_pressed) {
        swipe_start_position_.x = x;
        swipe_start_position_.y = y;
    } else {
        const int delta_pos = 300;
        auto swipe_direction = utils::math::Vector2f(0, 0);

        auto current_delta_x = x - swipe_start_position_.x;
        auto current_delta_y = y - swipe_start_position_.y;
        if (std::abs(current_delta_x) > delta_pos) {
            swipe_direction.x = current_delta_x > 0 ? 1 : -1;
        }

        if (std::abs(current_delta_y) > delta_pos) {
            swipe_direction.y = current_delta_y > 0 ? 1 : -1;
        }

        const SwipeData data = SwipeData{swipe_direction};
        for (auto value : swipe_events_) {
            value.second->call(data);
        }
    }
}


void InputManager::handleContinuousKeyEvent() {
    if (event_handlers_locked_) {
        return;
    }

    if (continuous_key_events_.size() == 0) {
        return;
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

    for (auto key_event : continuous_key_events_) {
        unsigned int sdl_key_event = continuous_key_sdl_connector_[key_event.first];

        if (state[sdl_key_event]) {
            auto key_map = key_event.second;
            for (auto value : *key_map) {
                value.second->call({true});
            }   
        }
    }
}

bool InputManager::hasKeyEvents() const {
    return key_events_.size() > 0;
}

bool InputManager::hasMouseMoveEvents() const {
    return mouse_events_.size() > 0;
}

bool InputManager::hasMouseEvents() const {
    return mouse_events_.size() > 0;

}
bool InputManager::hasTouchEvents() const {
    return mouse_events_.size() > 0;

}
bool InputManager::hasSwipeEvents() const {
    return swipe_events_.size() > 0;
}


}
}