#include "alisa/core/logger.h"

int main(int argc, char* argv[]) {
    alisa::core::Logger::init();

    alisa::core::Logger::info("init game %s", "test1");

    return 0;
}