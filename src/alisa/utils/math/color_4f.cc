#include "color_4f.h"

namespace alisa {
namespace utils {
namespace math {

Color4f::Color4f(float r, float g, float b, float a): 
    r(r), g(g), b(b), a(a) {}

Color4f::operator Vector4f() const {
    return {r, g, b, a};
}

bool Color4f::operator==(const Color4f& rhs) const {
    return (r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a);
}

bool Color4f::operator!=(const Color4f& rhs) const {
    return (r != rhs.r) || (g != rhs.g) || (b != rhs.b) || (a != rhs.a);
}

Color4f Color4f::operator+(const Color4f& rhs) const {
    return Color4f(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
}

Color4f Color4f::operator*(float scalar) const {
    return Color4f(r * scalar, g * scalar, b * scalar, a * scalar);
}   

const Color4f WhiteColor4f(1.0f, 1.0f, 1.0f, 1.0f);
const Color4f BlackColor4f(0.0f, 0.0f, 0.0f, 1.0f);
const Color4f RedColor4f(1.0f, 0.0f, 0.0f, 1.0f);
const Color4f GreenColor4f(0.0f, 1.0f, 0.0f, 1.0f);
const Color4f BlueColor4f(0.0f, 0.0f, 1.0f, 1.0f);
const Color4f SentinelColor4f(-1.0f, -1.0f, -1.0f, -1.0f);

}
}
}