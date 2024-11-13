#ifndef ALISA_UTILS_STRING_ID_H_
#define ALISA_UTILS_STRING_ID_H_

#include <string>
#include <cstdint>
#include <array>

namespace alisa {
namespace utils {

typedef uint32_t StringID;

StringID GetStringID(const std::string& data);

}    
}

#endif