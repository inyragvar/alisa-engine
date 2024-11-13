#ifndef ALISA_UTILS_MATH_FUNCTION_H_
#define ALISA_UTILS_MATH_FUNCTION_H_

#include "vector_2f.h"

namespace alisa {
namespace utils {
namespace math {

bool IsIntersect2D(float x, float y, float x0, float y0, float x1, float y1);
bool IsIntersect2D(const Vector2f& pos, const Vector2f& left_bottom, const Vector2f& top_right);

inline bool IsPowerOfTwo(int num) {
    return num && ((num & (num - 1)) == 0);
}

inline bool IsPowerOfTwo(int width, int height) {
    return IsPowerOfTwo(width) && IsPowerOfTwo(height);   
}

// Linear interpolation function
template<typename T>
T LinearInterpolation(const T& start, const T& end, double t) {
    return start + (end - start) * t;
}

}
}
}

#endif