#include "vector_3f.h"

#include <cmath>
#include "alisa/logger/logger.h"

#include "quaternion.h"

namespace alisa {
namespace utils {   
namespace math {

Vector3f::Vector3f() {
}

Vector3f::Vector3f(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

Vector3f::Vector3f(const float* pFloat) {
    x = pFloat[0];
    y = pFloat[0];
    z = pFloat[0];
}

Vector3f::Vector3f(float f) { x = y = z = f; }

Vector3f Vector3f::operator+(const Vector3f& r) const {
    Vector3f res;
    res.x = x + r.x;
    res.y = y + r.y;
    res.z = z + r.z;
    
    return res;
}

Vector3f& Vector3f::operator+=(const Vector3f& r) {
    x += r.x;
    y += r.y;
    z += r.z;

    return *this;
}

Vector3f Vector3f::operator-(const Vector3f& r) const {
    Vector3f res;
    res.x = x - r.x;
    res.y = y - r.y;
    res.z = z - r.z;
    
    return res;
}

Vector3f& Vector3f::operator-=(const Vector3f& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;

    return *this;
}

Vector3f Vector3f::operator*(const Vector3f& r) const {
    return Vector3f(x * r.x, y * r.y, z * r.z);
}

Vector3f& Vector3f::operator*=(const Vector3f& r) {
    x *= r.x;
    y *= r.y;
    z *= r.z;
    return *this;
}

Vector3f Vector3f::operator*(float f) const {
    Vector3f res;
    res.x = x * f;
    res.y = y * f;
    res.z = z * f;
    
    return res;
}

Vector3f& Vector3f::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;

    return *this;
}

Vector3f Vector3f::operator*(double d) const {
    Vector3f res;
    res.x = x * d;
    res.y = y * d;
    res.z = z * d;
    
    return res;
}

Vector3f& Vector3f::operator*=(double d) {
    x *= d;
    y *= d;
    z *= d;

    return *this;
}

Vector3f Vector3f::operator+(double d) const {
    return Vector3f(x + d, y + d, z + d);
}

Vector3f::operator const float*() const { 
    return &(x);
}

Vector3f Vector3f::cross(const Vector3f& v) const {
    const float _x = y * v.z - z * v.y;
    const float _y = z * v.x - x * v.z;
    const float _z = x * v.y - y * v.x;

    return Vector3f(_x, _y, _z);
}

Vector3f& Vector3f::normalize() {
    float len = length();
    if (len == 0) {
        return *this;
    }

    x /= len;
    y /= len;
    z /= len;

    return *this;
}

float Vector3f::length() const {
    return sqrtf(x * x + y * y + z * z);
}

void Vector3f::rotate(float angle, const Vector3f& axis) {
    Quaternion rotation(angle, axis);

    Quaternion conjugate = rotation.conjugate();

    Quaternion w = rotation * (*this) * conjugate;

    x = w.x;
    y = w.y;
    z = w.z;
}

float Vector3f::dot(const Vector3f& r) const {
    return x * r.x + y * r.y + z * r.z;
}

void Vector3f::print() const { 
    logger::Logger::info("(%.02f, %.02f, %.02f)", x, y, z); 
}


float Vector3f::distanceSquared(const Vector3f& other) const {
    return (x - other.x) * (x - other.x) +
           (y - other.y) * (y - other.y) +
           (z - other.z) * (z - other.z);
}

const Vector3f ZeroVector3f(0.0f, 0.0f, 0.0f);
const Vector3f UnitVector3f(1.0f, 1.0f, 1.0f);

const Vector3f XAxisVector3f(1.0f, 0.0f, 0.0f);
const Vector3f YAxisVector3f(0.0f, 1.0f, 0.0f);
const Vector3f ZAxisVector3f(0.0f, 0.0f, 1.0f);

}
}
}