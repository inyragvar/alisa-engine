#ifndef ALISA_UTILS_CONFIG_H_
#define ALISA_UTILS_CONFIG_H_

#include <string>
#include <mutex>

#include "alisa/utils/parser/toml/parser.h"

namespace alisa {
namespace utils {

class Config {
public:
    static Config& get(const char* file_path = nullptr);

public:
    ~Config();

public:
    int getInt(std::string key) const;
    float getFloat(std::string key) const;
    std::string getStr(std::string key) const;
    bool getBool(std::string key) const;

public:
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

private: 
    Config(const char* file_path);
    static void initConfig(const char* file_path);

protected:
    static std::once_flag init_instance_flag_;
    static Config* instance_;
    
private:
    parser::TomlParser parser_;
};

}
}  // namespace alisa

#endif