#include "value.h"

#include <stdexcept>

namespace alisa {
namespace utils {
namespace parser {

TomlValue::TomlValue() : type(NONE) {}

TomlValue::TomlValue(const std::string& str) : str_value(str), type(STRING) {}

TomlValue::TomlValue(int i) : int_value(i), type(INT) {}

TomlValue::TomlValue(float f) : float_value(f), type(FLOAT) {}

TomlValue::TomlValue(bool b) : bool_value(b), type(BOOL) {}

TomlValue::TomlValue(const math::Vector3f& vec) : vec_value(vec), type(VECTOR3F) {}

TomlValue::TomlValue(const std::map<std::string, TomlValue>& table) : table_value(table), type(TABLE) {}

TomlValue::TomlValue(const std::vector<TomlValue>& array) : array_value(array), type(ARRAY) {}

std::string TomlValue::asString() const { 
    if (type == STRING) return str_value; 
    throw std::runtime_error("Value is not a string.");
}

int TomlValue::asInt() const { 
    if (type == INT) return int_value; 
    throw std::runtime_error("Value is not an int.");
}

float TomlValue::asFloat() const { 
    if (type == FLOAT) return float_value; 
    throw std::runtime_error("Value is not a float.");
}

bool TomlValue::asBool() const { 
    if (type == BOOL) return bool_value; 
    throw std::runtime_error("Value is not a bool.");
}

math::Vector3f TomlValue::asVector3f() const {
    if (type == VECTOR3F) return vec_value;
    throw std::runtime_error("Value is not a vector3f.");
}

std::map<std::string, TomlValue> TomlValue::asTable() const {
    if (type == TABLE) return table_value;
    throw std::runtime_error("Value is not a table.");
}

std::vector<TomlValue> TomlValue::asArray() const {
    if (type == ARRAY) return array_value;
    throw std::runtime_error("Value is not an array.");
}

bool TomlValue::isNone() const {
    return type == NONE;
}

}
}
}