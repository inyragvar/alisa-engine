#include "matrix_4f.h"

#include "alisa/logger/logger.h"

#include "math.h"

namespace alisa {
namespace utils {
namespace math {

// init identity matrix - with1.0f
Matrix4f::Matrix4f() {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;  // Column 0
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;  // Column 1
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;  // Column 2
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;  // Column 3
}

// init identity matrix
Matrix4f::Matrix4f(float a) {
    m[0][0] = a;    m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;  // Column 0
    m[1][0] = 0.0f; m[1][1] = a;    m[1][2] = 0.0f; m[1][3] = 0.0f;  // Column 1
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = a;    m[2][3] = 0.0f;  // Column 2
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = a;     // Column 3
}

Matrix4f::Matrix4f(float a00, float a01, float a02, float a03,
                   float a10, float a11, float a12, float a13,
                   float a20, float a21, float a22, float a23,
                   float a30, float a31, float a32, float a33) {
    m[0][0] = a00; m[0][1] = a10; m[0][2] = a20; m[0][3] = a30;  // Column 0
    m[1][0] = a01; m[1][1] = a11; m[1][2] = a21; m[1][3] = a31;  // Column 1
    m[2][0] = a02; m[2][1] = a12; m[2][2] = a22; m[2][3] = a32;  // Column 2
    m[3][0] = a03; m[3][1] = a13; m[3][2] = a23; m[3][3] = a33;  // Column 3
}

Matrix4f Matrix4f::transpose() const {
    Matrix4f n;

    for (unsigned int i = 0 ; i < 4 ; i++) {
        for (unsigned int j = 0 ; j < 4 ; j++) {
            n.m[i][j] = m[j][i];
        }
    }

    return n;
}
void Matrix4f::identity(float v) {
    m[0][0] = v;    m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;  // Column 0
    m[1][0] = 0.0f; m[1][1] = v;    m[1][2] = 0.0f; m[1][3] = 0.0f;  // Column 1
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = v;    m[2][3] = 0.0f;  // Column 2
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = v;     // Column 3
}

void Matrix4f::scale(float s) {; 
    Matrix4f sc(1.0f);

    sc.m[0][0] = s;
    sc.m[1][1] = s; 
    sc.m[2][2] = s;
    
    *this = (*this) * sc; 
}

void Matrix4f::scale(const Vector3f& s) {; 
    Matrix4f sc(1.0f);

    sc.m[0][0] = s.x;
    sc.m[1][1] = s.y; 
    sc.m[2][2] = s.z;
    
    *this = (*this) * sc; 
}

void Matrix4f::scale(float x, float y, float z) {
    Matrix4f sc(1.0f);

    sc.m[0][0] = x;
    sc.m[1][1] = y; 
    sc.m[2][2] = z;

    *this = (*this) * sc;
}

void Matrix4f::rotate(const Vector3f& r) {
    Matrix4f rx(1.0f), ry(1.0f), rz(1.0f);

    const float angle_x = ToRadian(r.x);
    const float angle_y = ToRadian(r.y);
    const float angle_z = ToRadian(r.z);

    // Initialize rotation matrix around the X-axis (Column-Major Order)
    rx.m[1][1] = cosf(angle_x); 
    rx.m[1][2] = sinf(angle_x);

    rx.m[2][1] = -sinf(angle_x); 
    rx.m[2][2] = cosf(angle_x); 
  

    // Initialize rotation matrix around the Y-axis (Column-Major Order)
    ry.m[0][0] = cosf(angle_y); 
    ry.m[0][2] = -sinf(angle_y);

    ry.m[2][0] = sinf(angle_y); 
    ry.m[2][2] = cosf(angle_y);
   
    // Initialize rotation matrix around the Z-axis (Column-Major Order)
    rz.m[0][0] = cosf(angle_z);  
    rz.m[0][1] = sinf(angle_z); 

    rz.m[1][0] = -sinf(angle_z); 
    rz.m[1][1] = cosf(angle_z); 

    // Combine the rotations: rotation around Z, then Y, then X
    *this = (*this) * rz * ry * rx;
}

void Matrix4f::rotate(float x, float y, float z) {
    Matrix4f rx(1.0f), ry(1.0f), rz(1.0f);

    const float angle_x = ToRadian(x);
    const float angle_y = ToRadian(y);
    const float angle_z = ToRadian(z);

    // Initialize rotation matrix around the X-axis (Column-Major Order)
    rx.m[1][1] = cosf(angle_x); 
    rx.m[1][2] = sinf(angle_x);

    rx.m[2][1] = -sinf(angle_x); 
    rx.m[2][2] = cosf(angle_x); 
  

    // Initialize rotation matrix around the Y-axis (Column-Major Order)
    ry.m[0][0] = cosf(angle_y); 
    ry.m[0][2] = -sinf(angle_y);

    ry.m[2][0] = sinf(angle_y); 
    ry.m[2][2] = cosf(angle_y);
   
    // Initialize rotation matrix around the Z-axis (Column-Major Order)
    rz.m[0][0] = cosf(angle_z);  
    rz.m[0][1] = sinf(angle_z); 

    rz.m[1][0] = -sinf(angle_z); 
    rz.m[1][1] = cosf(angle_z); 

    // Combine the rotations: rotation around Z, then Y, then X
    *this = (*this) * rz * ry * rx;
}

void Matrix4f::rotateX(float x) {
    float angle = ToRadian(x);
    Matrix4f rx(1.0f);

    rx.m[1][1] = cosf(angle);  
    rx.m[1][2] = sinf(angle);  

    rx.m[2][1] = -sinf(angle); 
    rx.m[2][2] = cosf(angle);

    *this = (*this) * rx;
} 
    

void Matrix4f::rotateY(float y) {
    float angle = ToRadian(y);
    Matrix4f ry(1.0f);

    ry.m[0][0] = cosf(angle); 
    ry.m[0][2] = -sinf(angle);

    ry.m[2][0] = sinf(angle); 
    ry.m[2][2] = cosf(angle);

    *this = (*this) * ry;
}

void Matrix4f::rotateZ(float z) {
    float angle = ToRadian(z);
    Matrix4f rz(1.0f);

    rz.m[0][0] = cosf(angle);  
    rz.m[0][1] = sinf(angle); 

    rz.m[1][0] = -sinf(angle); 
    rz.m[1][1] = cosf(angle); 

    *this = (*this) * rz;
}

void Matrix4f::translate(const Vector3f& p) {
    Matrix4f tr(1.0f);

    tr.m[3][0] = p.x;
    tr.m[3][1] = p.y;
    tr.m[3][2] = p.z;

    *this = (*this) * tr;
}

void Matrix4f::translate(float x, float y, float z) {
   Matrix4f tr(1.0f);
    
    tr.m[3][0] = x;
    tr.m[3][1] = y;
    tr.m[3][2] = z;

    *this = (*this) * tr;
}

Matrix4f Matrix4f::operator*(const Matrix4f& right) const {
    Matrix4f result;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            result.m[col][row] = 
                m[0][row] * right.m[col][0] +
                m[1][row] * right.m[col][1] +
                m[2][row] * right.m[col][2] +
                m[3][row] * right.m[col][3];
        }
    }
    return result;
}

Vector4f Matrix4f::operator*(const Vector4f& v) const {
    Vector4f res;

    // Matrix-Vector multiplication for column-major order
    res.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0] * v.w;  // Dot product with first column
    res.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1] * v.w;  // Dot product with second column
    res.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2] * v.w;  // Dot product with third column
    res.w = m[0][3] * v.x + m[1][3] * v.y + m[2][3] * v.z + m[3][3] * v.w;  // Dot product with fourth column

    return res;
}

// Overload operator[] to access rows of the matrix
float* Matrix4f::operator[](int row) {
    return m[row];  // Return reference to the row
}

const float* Matrix4f::operator[](int row) const {
    return m[row];  // Return const reference to the row
}

Matrix4f::operator const float*() const {
    return &(m[0][0]);
}

void Matrix4f::print() const {
    for (int i = 0 ; i < 4 ; i++) {
        logger::Logger::info("%f %f %f %f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
    }
}

float Matrix4f::determinant() const {
    return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][2]*m[2][1]*m[3][3] + m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][1]*m[2][3]*m[3][2]
         + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][2]*m[2][0]*m[3][3]
         - m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][0]*m[2][3]*m[3][2] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][3]*m[2][2]*m[3][0]
         + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][1]*m[2][0]*m[3][3] + m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][0]*m[2][3]*m[3][1]
         + m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][3]*m[2][1]*m[3][0] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][1]*m[2][0]*m[3][2]
         - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][0]*m[2][2]*m[3][1] - m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
}

Matrix4f Matrix4f::inverse() const {
    float det = determinant();

    if (det == 0.0f) {
        logger::Logger::error("Matrix4f determinant == 0");
        return *this; // Return the original matrix if it's not invertible
    }

    float invdet = 1.0f / det;
    Matrix4f res;

    res.m[0][0] = invdet * (m[1][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[2][1] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) + m[3][1] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]));
    res.m[0][1] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[2][0] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) + m[3][0] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]));
    res.m[0][2] = invdet * (m[1][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) - m[2][0] * (m[1][1] * m[3][3] - m[3][1] * m[1][3]) + m[3][0] * (m[1][1] * m[2][3] - m[2][1] * m[1][3]));
    res.m[0][3] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) - m[2][0] * (m[1][1] * m[3][2] - m[3][1] * m[1][2]) + m[3][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]));

    res.m[1][0] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[2][1] * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) + m[3][1] * (m[0][2] * m[2][3] - m[2][2] * m[0][3]));
    res.m[1][1] = invdet * (m[0][0] * (m[2][2] * m[3][3] - m[3][2] * m[2][3]) - m[2][0] * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) + m[3][0] * (m[0][2] * m[2][3] - m[2][2] * m[0][3]));
    res.m[1][2] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[3][1] * m[2][3]) - m[2][0] * (m[0][1] * m[3][3] - m[3][1] * m[0][3]) + m[3][0] * (m[0][1] * m[2][3] - m[2][1] * m[0][3]));
    res.m[1][3] = invdet * (m[0][0] * (m[2][1] * m[3][2] - m[3][1] * m[2][2]) - m[2][0] * (m[0][1] * m[3][2] - m[3][1] * m[0][2]) + m[3][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]));

    res.m[2][0] = invdet * (m[0][1] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) - m[1][1] * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) + m[3][1] * (m[0][2] * m[1][3] - m[1][2] * m[0][3]));
    res.m[2][1] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[3][2] * m[1][3]) - m[1][0] * (m[0][2] * m[3][3] - m[3][2] * m[0][3]) + m[3][0] * (m[0][2] * m[1][3] - m[1][2] * m[0][3]));
    res.m[2][2] = invdet * (m[0][0] * (m[1][1] * m[3][3] - m[3][1] * m[1][3]) - m[1][0] * (m[0][1] * m[3][3] - m[3][1] * m[0][3]) + m[3][0] * (m[0][1] * m[1][3] - m[1][1] * m[0][3]));
    res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[3][1] * m[1][2]) - m[1][0] * (m[0][1] * m[3][2] - m[3][1] * m[0][2]) + m[3][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

    res.m[3][0] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]) - m[1][1] * (m[0][2] * m[2][3] - m[2][2] * m[0][3]) + m[2][1] * (m[0][2] * m[1][3] - m[1][2] * m[0][3]));
    res.m[3][1] = invdet * (m[0][0] * (m[1][2] * m[2][3] - m[2][2] * m[1][3]) - m[1][0] * (m[0][2] * m[2][3] - m[2][2] * m[0][3]) + m[2][0] * (m[0][2] * m[1][3] - m[1][2] * m[0][3]));
    res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[2][1] * m[1][3]) - m[1][0] * (m[0][1] * m[2][3] - m[2][1] * m[0][3]) + m[2][0] * (m[0][1] * m[1][3] - m[1][1] * m[0][3]));
    res.m[3][3] = invdet * (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2]) + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

    return res;
}

void ConvertToColumnMajor(const Matrix4f& mat, float* column_major_array) {
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            column_major_array[col * 4 + row] = mat.m[row][col];
        }
    }
}

}
}
}