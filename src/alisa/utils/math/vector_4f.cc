#include "vector_4f.h"

#include <cmath>
#include "alisa/logger/logger.h"

namespace alisa {
namespace utils {
namespace math {

Vector4f::Vector4f() {
}

Vector4f::Vector4f(float _x, float _y, float _z, float _w) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vector4f::Vector4f(const float* f) {
    x = f[0];
    y = f[0];
    z = f[0];
    w = f[0];
}

Vector4f::Vector4f(float f) { 
    x = y = z = w = f; 
}

Vector4f& Vector4f::operator=(const Vector4f& r) {
    x = r.x;
    y = r.y;
    z = r.z;
    w = r.w;

    return *this;
}

Vector4f Vector4f::operator+(const Vector4f& r) const {
    Vector4f res;
    res.x = x + r.x;
    res.y = y + r.y;
    res.z = z + r.z;
    res.w = w + r.w;
    
    return res;
}

Vector4f& Vector4f::operator+=(const Vector4f& r) {
    x += r.x;
    y += r.y;
    z += r.z;
    w += r.w;

    return *this;
}

Vector4f Vector4f::operator-(const Vector4f& r) const {
    Vector4f res;
    res.x = x - r.x;
    res.y = y - r.y;
    res.z = z - r.z;
    res.w = w - r.w;
    
    return res;
}

Vector4f& Vector4f::operator-=(const Vector4f& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    w -= r.w;

    return *this;
}

Vector4f Vector4f::operator*(float f) const {
    Vector4f res;
    res.x = x * f;
    res.y = y * f;
    res.z = z * f;
    res.w = w * f;
    
    return res;
}

Vector4f& Vector4f::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    w *= f;

    return *this;
}

Vector4f Vector4f::operator*(double d) const {
    Vector4f res;
    res.x = x * d;
    res.y = y * d;
    res.z = z * d;
    res.w = w * d;
    
    return res;
}

Vector4f& Vector4f::operator*=(double d) {
    x *= d;
    y *= d;
    z *= d;
    w *= d;

    return *this;
}

Vector4f Vector4f::operator+(double d) const {
    return Vector4f(x + d, y + d, z + d, w + d);
}

Vector4f::operator const float*() const { return &(x); }

Vector4f Vector4f::cross(const Vector4f& v) const {
    return Vector4f(y * v.z - z * v.y, 
                    z * v.x - x * v.z, 
                    x * v.y - y * v.x,
                    1.0f);
}

Vector4f& Vector4f::normalize() {
    float len = length();

    x /= len;
    y /= len;
    z /= len;
    w /= len;

    return *this;
}

float Vector4f::length() const {
    return sqrtf(x * x + y * y + z * z + w * w);
}

void Vector4f::rotate(float angle, const Vector4f& axis) {}

void Vector4f::print() const { 
    logger::Logger::info("(%.02f, %.02f, %.02f)", x, y, z); 
}

}
}
}