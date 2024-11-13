#include "vector_2f.h"

namespace alisa {
namespace utils {
namespace math {

Vector2f::Vector2f() {
}

Vector2f::Vector2f(float x, float y):
    u(x), v(y)  {
}

bool Vector2f::operator==(const Vector2f& rhs) const {
    return (u == rhs.u) && (v == rhs.v);
}

bool Vector2f::operator!=(const Vector2f& rhs) const {
    return (u != rhs.u) || (v != rhs.v);
}

Vector2f Vector2f::operator-(const Vector2f& rhs) const {
    return Vector2f(x - rhs.x, y - rhs.y);
}

Vector2f Vector2f::operator+(const Vector2f& rhs) const {
    return Vector2f(x + rhs.x, y + rhs.y);
}

Vector2f& Vector2f::operator+=(const Vector2f& r) {
    x += r.x;
    y += r.y;

    return *this;
}

Vector2f operator*(const Vector2f& l, float f) {
    Vector2f res(l.x * f, l.y * f);

    return res;
}

const Vector2f SentinelVector2f(-1.0f, -1.0f);

}
}
}