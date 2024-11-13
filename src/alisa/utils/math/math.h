#ifndef ALISA_UTILS_MATH_MATH_H_
#define ALISA_UTILS_MATH_MATH_H_

#include <cmath>

namespace alisa {
namespace utils {
namespace math {
    
// Define PI as a constexpr variable instead of a macro
constexpr double PI = 3.14159265358979323846;

// Convert degrees to radians using an inline function
inline float ToRadian(float degrees) {
    return static_cast<float>(degrees * PI / 180.0);
}

// Convert radians to degrees using an inline function
inline float ToDegree(float radians) {
    return static_cast<float>(radians * 180.0 / PI);
}

}
}
}

#endif