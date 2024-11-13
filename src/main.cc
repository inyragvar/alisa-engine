#include "alisa/core/logger.h"
#include "alisa/utils/config.h"

int main(int argc, char* argv[]) {
    alisa::core::Logger::init();

    auto& cfg = alisa::utils::Config::get("config/dev.toml");

    alisa::core::Logger::info("init game %s", cfg.getStr("game_title").c_str());

    return 0;
}