#ifndef ALISA_UTILS_MATH_COLOR_4F_H_
#define ALISA_UTILS_MATH_COLOR_4F_H_

#include "vector_4f.h"

namespace alisa {
namespace utils {
namespace math {

struct Color4f {    
    float r;
    float g;
    float b;
    float a;

    Color4f(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);

    operator Vector4f() const;
    bool operator==(const Color4f& rhs) const;    
    bool operator!=(const Color4f& rhs) const;

    Color4f operator+(const Color4f& rhs) const;    

    // Multiplication operator with a float scalar
    Color4f operator*(float scalar) const;
};

extern const Color4f WhiteColor4f;
extern const Color4f BlackColor4f;
extern const Color4f RedColor4f;
extern const Color4f GreenColor4f;
extern const Color4f BlueColor4f;
extern const Color4f SentinelColor4f;

}
}
}

#endif