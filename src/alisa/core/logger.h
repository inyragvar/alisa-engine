#ifndef ALISA_CORE_LOGGER_H_
#define ALISA_CORE_LOGGER_H_

#include <string>

#include "lib/SDL2/SDL.h"

namespace alisa {
namespace core {

class Logger {
public:
    // Logging levels
    enum class Level {
        INFO,
        WARN,
        ERROR,
        DEBUG
    };

    // Initializes the logger with an optional tag
    static void init();

    // Logs a message at the specified level
    static void log(Level level, const char* fmt, ...);

    // Convenience functions for specific levels
    static void info(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void error(const char* fmt, ...);
    static void debug(const char* fmt, ...);

private:
    static void sdlLog(Level level, const char* fmt, va_list args);
    
};

}
}

#endif // LOGGER_H
