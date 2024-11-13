#ifndef ALISA_UTILS_MATH_MATRIX_4F_H_
#define ALISA_UTILS_MATH_MATRIX_4F_H_

#include "vector_3f.h"
#include "vector_4f.h"

namespace alisa {
namespace utils {
namespace math {

struct Matrix4f {
    float m[4][4];

    Matrix4f();
    // init identity matrix
    Matrix4f(float a);

    Matrix4f(float a00, float a01, float a02, float a03,
             float a10, float a11, float a12, float a13,
             float a20, float a21, float a22, float a23,
             float a30, float a31, float a32, float a33);

    Matrix4f transpose() const;

    void identity(float v = 1.0f);
    
    // Transformations  M = T × R × S
    // so translate(), rotate(), scale()
    //
    void scale(float s);
    void scale(const Vector3f& s);
    void scale(float x, float y, float z);
    
    void rotate(const Vector3f& angle);
    void rotate(float x, float y, float z);

    void rotateX(float x);
    void rotateY(float y);
    void rotateZ(float z);

    void translate(const Vector3f& postion);
    void translate(float x, float y, float z);

    Matrix4f operator*(const Matrix4f& right) const;
    Vector4f operator*(const Vector4f& v) const;

    float* operator[](int row);
    
    operator const float*() const;
    const float* operator[](int row) const;
    
    void print() const;

    float determinant() const;

    Matrix4f inverse() const;
};

void ConvertToColumnMajor(const Matrix4f& mat, float* column_major_array);

}
}
}

#endif