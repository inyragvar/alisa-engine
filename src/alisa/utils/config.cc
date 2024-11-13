#include "config.h"

#include "file.h"

#include "alisa/logger/logger.h"

namespace alisa {
namespace utils {

Config* Config::instance_ = nullptr;
std::once_flag Config::init_instance_flag_;

void Config::initConfig(const char* file_path) {
    instance_ = new Config(file_path);
}

Config& Config::get(const char* file_path) {
    std::call_once(init_instance_flag_, &Config::initConfig, file_path);

    return *instance_;
}

Config::Config(const char* file_path) {
    std::string file_content = ReadFileStr(file_path);
    parser_.parse(file_content);

    // core::Logger::info("%s\n", file_content.c_str());
}

int Config::getInt(std::string key) const {
    return parser_.get(key).asInt();
}

float Config::getFloat(std::string key) const {
    return parser_.get(key).asFloat();
}

std::string Config::getStr(std::string key) const {
    return parser_.get(key).asString();
}

bool Config::getBool(std::string key) const {
    return parser_.get(key).asBool();
}

}
}