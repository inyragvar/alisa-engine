#ifndef ALISA_UTILS_MATH_VECTOR_4F_H_
#define ALISA_UTILS_MATH_VECTOR_4F_H_

namespace alisa {
namespace utils {
namespace math {

struct Vector4f {
    float x;
    float y;
    float z;
    float w;

    Vector4f();
    Vector4f(float _x, float _y, float _z, float _w);
    Vector4f(const float* f);
    Vector4f(float f);

    Vector4f& operator=(const Vector4f& r);
    Vector4f operator+(const Vector4f& r) const;
    Vector4f& operator+=(const Vector4f& r);
    Vector4f operator-(const Vector4f& r) const;
    Vector4f& operator-=(const Vector4f& r);
    Vector4f operator*(float f) const;
    Vector4f& operator*=(float f);
    Vector4f operator*(double d) const;
    Vector4f& operator*=(double d);

    Vector4f operator+(double d) const;
    
    operator const float*() const;

    Vector4f cross(const Vector4f& v) const;
    Vector4f& normalize();

    float length() const;
    void rotate(float angle, const Vector4f& axis);
    void print() const;
};

}
}
}

#endif