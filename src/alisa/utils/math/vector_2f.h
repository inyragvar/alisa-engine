#ifndef ALISA_UTILS_MATH_VECTOR_2F_H_
#define ALISA_UTILS_MATH_VECTOR_2F_H_

namespace alisa {
namespace utils {
namespace math {

struct Vector2f {
    union {
        float x;
        float u;
    };

    union {
        float y;
        float v;
    };

    Vector2f();
    Vector2f(float x, float y);

    bool operator==(const Vector2f& rhs) const;    
    bool operator!=(const Vector2f& rhs) const;  
    
    Vector2f operator-(const Vector2f& rhs) const;  
    Vector2f operator+(const Vector2f& rhs) const;
    Vector2f& operator+=(const Vector2f& r);

    Vector2f operator/(float f) const; 
    Vector2f& operator/=(float f);     
};


Vector2f operator*(const Vector2f& l, float f);

extern const Vector2f SentinelVector2f;

inline Vector2f Vector2f::operator/(float f) const {
    return Vector2f(x / f, y / f);
}

inline Vector2f& Vector2f::operator/=(float f) {
    x /= f;
    y /= f;
    return *this;
}

}
}
}

#endif