#include "function.h"

namespace alisa {
namespace utils {
namespace math {

bool IsIntersect2D(float x, float y, float x0, float y0, float x1, float y1) {
    return (x >= x0) && (y >= y0) && (x <= x1) && (y <= y1);
}

bool IsIntersect2D(const Vector2f& pos, const Vector2f& left_bottom, const Vector2f& top_right) {
    return (pos.x >= left_bottom.x) && (pos.y >= left_bottom.y) && (pos.x <= top_right.x) && (pos.y <= top_right.y);
}

}
}
}