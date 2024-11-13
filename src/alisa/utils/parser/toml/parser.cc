#include "parser.h"

#include <functional>

namespace alisa {
namespace utils {
namespace parser { 

void TomlParser::parse(const std::string& file_content, bool is_assets) {
    std::istringstream file(file_content);
    std::string line;
    std::map<std::string, TomlValue> current_table;
    std::string current_table_name; 
    bool is_array_of_tables = false;

    std::function<TomlValue(std::string)> parse_values;
    parse_values = [this, &parse_values](std::string value_str) -> TomlValue {
        TomlValue value;
        if (value_str[0] == '"' && value_str.back() == '"') { // String
            value = TomlValue(value_str.substr(1, value_str.size() - 2));
        } else if (value_str[0] == '[' && value_str.back() == ']') { // Array
            std::vector<TomlValue> array;
            std::string array_content = value_str.substr(1, value_str.size() - 2); // Remove '[' and ']'
            std::istringstream array_ss(array_content);
            std::string item;

            while (std::getline(array_ss, item, ',')) {
                array.push_back(parse_values(trim(item))); 
            }

            value = TomlValue(array);
        } else if (value_str == "true" || value_str == "false") { // Boolean
            value = TomlValue(value_str == "true");
        } else if (value_str.find('.') != std::string::npos && value_str.find(',') == std::string::npos) { // Float
            value = TomlValue(std::stof(value_str));
        } else if (value_str.find(',') != std::string::npos) { // Vector3f (x, y, z)
            value = TomlValue(parseVector3f(value_str));
        } else { // Integer
            value = TomlValue(std::stoi(value_str));
        }

        return value;
    };

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Skip comments or empty lines

        // Check for new table or array of tables
        if (line[0] == '[' && line.back() == ']') {
            // previous regular table 
            if (!current_table_name.empty() && !is_array_of_tables) {
                if (current_table_name.find('.') != std::string::npos) {
                    // FIXME: add support for multi-level sub-tables
                    std::string ctn = current_table_name;

                    std::string parent_table = ctn.substr(0, ctn.find('.'));
                    std::string subtable_name = ctn.substr(ctn.find('.') + 1, ctn.size() - ctn.find('.') - 1);
                    
                    values[parent_table].table_value[subtable_name] = TomlValue(current_table); 
                } else {
                    values[current_table_name] = TomlValue(current_table);    
                }
            }

            // previous array element
            if (is_array_of_tables && !current_table_name.empty()) {
                values[current_table_name].array_value.push_back(TomlValue(current_table));
            }

            if (current_table_name.empty()) {
                values.insert(current_table.begin(), current_table.end());
            }

            if (line[1] == '[') { // Array of tables
                std::string table_name = line.substr(2, line.size() - 4); // [[table_name]]
                
                current_table_name = table_name;
                current_table.clear();
                is_array_of_tables = true;

                // Initialize or append to the array
                if (values.find(current_table_name) == values.end()) {
                    values[current_table_name] = TomlValue(std::vector<TomlValue>()); // Init array
                }
            } else { // Regular table
                current_table_name = line.substr(1, line.size() - 2); // [table_name]
                current_table.clear();
                is_array_of_tables = false;
            }
        } else {
            auto pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = trim(line.substr(0, pos));
                std::string value_str = trim(line.substr(pos + 1));

                current_table[key] = parse_values(value_str);
            }
        }
    }

    // Save the last regular table
    if (!current_table_name.empty() && !is_array_of_tables) {
        if (current_table_name.find('.') != std::string::npos) {
            std::string ctn = current_table_name;
            std::string parent_table = ctn.substr(0, ctn.find('.'));
            std::string subtable_name = ctn.substr(ctn.find('.') + 1, ctn.size() - ctn.find('.') - 1);
                        
            values[parent_table].table_value[subtable_name] = TomlValue(current_table); 
        } else {
            values[current_table_name] = TomlValue(current_table);    
        }
}

    if (is_array_of_tables && !current_table_name.empty()) {
        values[current_table_name].array_value.push_back(TomlValue(current_table));
    }

    if (current_table_name.empty()) {
        values.insert(current_table.begin(), current_table.end());
    }
}

TomlValue TomlParser::get(const std::string& key) const {
    auto it = values.find(key);
    if (it != values.end()) {
        return it->second;
    }
    throw std::runtime_error("Key not found: " + key);
}

// Helper function to parse a vector3f from a string "(x, y, z)"
math::Vector3f TomlParser::parseVector3f(const std::string& str) const {
    math::Vector3f vec;
    std::sscanf(str.c_str(), "(%f,%f,%f)", &vec.x, &vec.y, &vec.z);
    return vec;
}

// Helper function to trim whitespace from both sides of a string
std::string TomlParser::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

}
}
}