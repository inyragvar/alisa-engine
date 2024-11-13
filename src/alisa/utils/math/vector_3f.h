#ifndef ALISA_UTILS_MATH_VECTOR_3F_H_
#define ALISA_UTILS_MATH_VECTOR_3F_H_

namespace alisa {
namespace utils {
namespace math {

struct Vector3f {
    float x;
    float y;
    float z;

    Vector3f();
    Vector3f(float _x, float _y, float _z);
    Vector3f(const float* pFloat);
    Vector3f(float f);

    Vector3f operator+(const Vector3f& r) const;
    Vector3f& operator+=(const Vector3f& r);
    Vector3f operator-(const Vector3f& r) const;
    Vector3f& operator-=(const Vector3f& r);
    Vector3f operator*(const Vector3f& r) const;
    Vector3f& operator*=(const Vector3f& r); 
    Vector3f operator*(float f) const;
    Vector3f& operator*=(float f);
    Vector3f operator*(double d) const;
    Vector3f& operator*=(double d);

    Vector3f operator+(double d) const;

    Vector3f operator/(float f) const; 
    Vector3f& operator/=(float f);      

    friend Vector3f operator*(float f, const Vector3f& v);  

    operator const float*() const;

    Vector3f cross(const Vector3f& v) const;
    Vector3f& normalize();

    float length() const;
    void rotate(float angle, const Vector3f& axis);

    float dot(const Vector3f& r) const;

    void print() const;

    float distanceSquared(const Vector3f& other) const;
    float squareLength() const;
};

extern const Vector3f ZeroVector3f;
extern const Vector3f UnitVector3f;

extern const Vector3f XAxisVector3f;
extern const Vector3f YAxisVector3f;
extern const Vector3f ZAxisVector3f;

// Implementation of operators
inline Vector3f Vector3f::operator/(float f) const {
    return Vector3f(x / f, y / f, z / f);
}

inline Vector3f& Vector3f::operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

inline Vector3f operator*(float f, const Vector3f& v) {
    return Vector3f(v.x * f, v.y * f, v.z * f);
}

inline float Vector3f::squareLength() const {
    return x*x + y*y + z*z;
}

}
}
}

#endif