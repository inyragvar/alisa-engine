#ifndef ALISA_UTILS_PARSER_TOML_TOML_PARSER_H_
#define ALISA_UTILS_PARSER_TOML_TOML_PARSER_H_

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "value.h"

#include "alisa/utils/math/vector_3f.h" 

namespace alisa { 
namespace utils {
namespace parser {   
 
class TomlParser {
public:
    std::map<std::string, TomlValue> values;

    void parse(const std::string& file_content, bool is_assets = true);
    TomlValue get(const std::string& key) const;

private:
    math::Vector3f parseVector3f(const std::string& str) const;

    // Helper function to trim whitespace from both sides of a string
    std::string trim(const std::string& str) const;
};

}
}
}

#endif