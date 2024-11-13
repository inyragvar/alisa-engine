#include "quaternion.h"

#include "math.h"

namespace alisa {
namespace utils {
namespace math {

Quaternion::Quaternion() {
}

Quaternion::Quaternion(float angle, const Vector3f& v) {
    float half_angle_in_radians = ToRadian(angle / 2);

    float sin_half_angle = sinf(half_angle_in_radians);
    float cos_half_angle = cosf(half_angle_in_radians);

    x = v.x * sin_half_angle;
    y = v.y * sin_half_angle;
    z = v.z * sin_half_angle;
    
    w = cos_half_angle;
}

Quaternion::Quaternion(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::normalize() {
    float Length = sqrtf(x * x + y * y + z * z + w * w);

    x /= Length;
    y /= Length;
    z /= Length;
    w /= Length;
}

Quaternion Quaternion::conjugate() const {
    Quaternion ret(-x, -y, -z, w);
    return ret;
}


Quaternion operator*(const Quaternion& q, const Vector3f& v) {
    float w = - (q.x * v.x) - (q.y * v.y) - (q.z * v.z);
    float x =   (q.w * v.x) + (q.y * v.z) - (q.z * v.y);
    float y =   (q.w * v.y) + (q.z * v.x) - (q.x * v.z);
    float z =   (q.w * v.z) + (q.x * v.y) - (q.y * v.x);

    Quaternion ret(x, y, z, w);

    return ret;
}


Quaternion operator*(const Quaternion& l, const Quaternion& r) {
    float w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
    float x = (l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y);
    float y = (l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z);
    float z = (l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x);

    Quaternion ret(x, y, z, w);

    return ret;
}

Vector3f Quaternion::toDegrees() {
    float f[3];

    f[0] = atan2(x * z + y * w, x * w - y * z);
    f[1] = acos(-x * x - y * y - z * z - w * w);
    f[2] = atan2(x * z - y * w, x * w + y * z);

    f[0] = ToDegree(f[0]);
    f[1] = ToDegree(f[1]);
    f[2] = ToDegree(f[2]);

    return Vector3f(f);
}

Matrix4f QuaternionToMatrix(const Quaternion& q) {
    Matrix4f m;
    m.m[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
    m.m[0][1] = 2 * q.x * q.y - 2 * q.z * q.w;
    m.m[0][2] = 2 * q.x * q.z + 2 * q.y * q.w;
    m.m[0][3] = 0;

    m.m[1][0] = 2 * q.x * q.y + 2 * q.z * q.w;
    m.m[1][1] = 1 - 2 * q.x * q.x - 2 * q.z * q.z;
    m.m[1][2] = 2 * q.y * q.z - 2 * q.x * q.w;
    m.m[1][3] = 0;

    m.m[2][0] = 2 * q.x * q.z - 2 * q.y * q.w;
    m.m[2][1] = 2 * q.y * q.z + 2 * q.x * q.w;
    m.m[2][2] = 1 - 2 * q.x * q.x - 2 * q.y * q.y;
    m.m[2][3] = 0;

    m.m[3][0] = 0;
    m.m[3][1] = 0;
    m.m[3][2] = 0;
    m.m[3][3] = 1;
    return m;
}

}
}
}