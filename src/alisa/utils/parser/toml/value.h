#ifndef ALISA_UTILS_PARSER_TOML_VALUE_H_
#define ALISA_UTILS_PARSER_TOML_VALUE_H_

#include <string>
#include <map>
#include <vector>

#include "alisa/utils/math/vector_3f.h" 

namespace alisa {
namespace utils {
namespace parser { 

// Simple data holder for TOML values
class TomlValue {
public:
    std::string str_value;
    int int_value;
    float float_value;
    bool bool_value;
    math::Vector3f vec_value;
    std::map<std::string, TomlValue> table_value;
    std::vector<TomlValue> array_value;

    enum ValueType { STRING, INT, FLOAT, BOOL, VECTOR3F, TABLE, ARRAY, NONE } type;

    TomlValue();
    TomlValue(const std::string& str);
    TomlValue(int i);
    TomlValue(float f);
    TomlValue(bool b);
    TomlValue(const math::Vector3f& vec);
    TomlValue(const std::map<std::string, TomlValue>& table);
    TomlValue(const std::vector<TomlValue>& array);

    std::string asString() const;
    int asInt() const;
    float asFloat() const;
    bool asBool() const;
    math::Vector3f asVector3f() const;
    std::map<std::string, TomlValue> asTable() const;
    std::vector<TomlValue> asArray() const;

    bool isNone() const;
};

}
}
}

#endif