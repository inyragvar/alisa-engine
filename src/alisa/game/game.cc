#include "game.h"

#include <chrono>
#include <thread>

#include "alisa/logger/logger.h"

#include "alisa/utils/config.h"
#include "alisa/screen/screen_manager.h"

namespace alisa {
namespace game {

Game::Game() {
}

Game::~Game() {}

bool Game::init() {
    auto& config = utils::Config::get();
    logger::Logger::info("width: %d, height: %d", config.getInt("game_screen_width"), config.getInt("game_screen_height"));


    return true;
}

bool Game::run() {
    bool is_running = true;

    auto& screen_manager = screen::ScreenManager::get();
    
    // Set the target frame rate (60 frames per second)
    const double target_frame_rate = utils::Config::get().getFloat("frame_rate");
    const std::chrono::duration<double> target_frame_time = std::chrono::duration<double>(1.0 / target_frame_rate);

    auto current_time = std::chrono::high_resolution_clock::now();
    auto prev_time = current_time;
    std::chrono::duration<double> dt;
    std::chrono::duration<double> zero_duration = std::chrono::duration<double>::zero();
    std::chrono::duration<double> second = zero_duration;
    int fps = 0;
    
    while (is_running) {
        // FIXME - add input_manager

        auto current_screen = screen_manager.getCurrent();

        current_time = std::chrono::high_resolution_clock::now();
        dt = current_time - prev_time;
        prev_time = current_time;

        if (dt < target_frame_time) {
            std::this_thread::sleep_for(target_frame_time - dt);
            dt = target_frame_time;
        }

        second += dt;
        if (second.count() > 1.0) {
            second = zero_duration;
            logger::Logger::info("fps: %d", fps);    
            fps = 0;
        }
        fps++;

        current_screen->update(dt.count());

        // render screen

        screen_manager.changeScreen();
        return true;
    }

    return true;
}

bool Game::destroy() {
    return true;
}

}
}  // namespace alisa