#include "logger.h"

namespace alisa {
namespace logger {

void Logger::init() {
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
}

// Logs a message with the given level
void Logger::log(Level level, const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    sdlLog(level, fmt, ap);
    va_end(ap);
}

void Logger::info(const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    sdlLog(Level::INFO, fmt, ap);
    va_end(ap);
}

void Logger::warn(const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    sdlLog(Level::WARN, fmt, ap);
    va_end(ap);
}

void Logger::error(const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    sdlLog(Level::ERROR, fmt, ap);
    va_end(ap);
}

void Logger::debug(const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    sdlLog(Level::DEBUG, fmt, ap);
    va_end(ap);
}

// Internal function to map Level to SDL_Log priority and log
void Logger::sdlLog(Level level, const char* fmt, va_list args) {
   
    SDL_LogPriority priority = SDL_LOG_PRIORITY_VERBOSE;

    switch (level) {
        case Level::INFO:
            priority = SDL_LOG_PRIORITY_INFO;
            break;
        case Level::WARN:
            priority = SDL_LOG_PRIORITY_WARN;
            break;
        case Level::ERROR:
            priority = SDL_LOG_PRIORITY_ERROR;
            break;
        case Level::DEBUG:
            priority = SDL_LOG_PRIORITY_DEBUG;
            break;
    }

    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, priority, fmt, args);
}

}
}