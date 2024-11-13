#ifndef ALISA_UTILS_MATH_QUATERNION_H_
#define ALISA_UTILS_MATH_QUATERNION_H_

#include "vector_3f.h"
#include "matrix_4f.h"

namespace alisa {
namespace utils {
namespace math {

struct Quaternion {
    float x, y, z, w;

    Quaternion();
    Quaternion(float angle, const Vector3f& v);

    Quaternion(float x, float y, float z, float w);

    void normalize();

    Quaternion conjugate() const;

    Vector3f toDegrees();
};

Quaternion operator*(const Quaternion& l, const Quaternion& r);

Quaternion operator*(const Quaternion& q, const Vector3f& v);


Matrix4f QuaternionToMatrix(const Quaternion& q);

}
}
}

#endif