#include "vector_3f_transform.h"

namespace alisa {
namespace utils {
namespace math {

Vector3f Normalize(const Vector3f& rhs) {
    float len = rhs.length();
    if (len == 0) {
        return rhs;
    }

    Vector3f ret;
    ret.x = rhs.x / len;
    ret.y = rhs.y / len;
    ret.z = rhs.z / len;

    return ret;
}

}
}
}