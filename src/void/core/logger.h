#ifndef VOID_CORE_LOGGER_H_
#define VOID_CORE_LOGGER_H_

#include <string>

#include "lib/SDL2/SDL.h"

namespace void {
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
    static void Init(const std::string& tag = "GameEngine");

    // Logs a message at the specified level
    static void Log(Level level, const std::string& message);

    // Convenience functions for specific levels
    static void Info(const std::string& message);
    static void Warn(const std::string& message);
    static void Error(const std::string& message);
    static void Debug(const std::string& message);

private:
    static void SDLLog(Level level, const std::string& message);

private:
    static std::string tag;
    
};

}
}

#endif // LOGGER_H
