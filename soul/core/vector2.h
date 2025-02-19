#pragma once

#include <math.h>

namespace soul {

template <typename T>
class Vector2 {
public:
    T x, y;

    Vector2() : x(0), y(0) {}
    Vector2(T x_, T y_) : x(x_), y(y_) {}
    ~Vector2() = default;

    Vector2(const Vector2<T>& o) {
        x = o.x;
        y = o.y;
    }

    float length() const
    {
        return sqrtf((x*x)+(y*y));
    }

    float dist(const Vector2<T>& v)
    {
        float dx = v.x - x;
        float dy = v.y - y;
        return sqrtf((dx*dx)+(dy*dy));
    }

    bool operator == (const Vector2<T>& v)
    {
        return (v.x == x) && (v.y == y);
    }

    void normalize()
    {
        x /= this->length();
        y /= this->length();
    }

};

template <typename T>
Vector2<T> operator+(Vector2<T>& v1, Vector2<T>& v2) {
    return Vector2<T>(v1.x+v2.x, v1.y+v2.y);
}

template <typename T>
Vector2<T> operator-(Vector2<T>& v1, Vector2<T>& v2) {
    return Vector2<T>(v1.x-v2.x, v1.y-v2.y);
}

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;

} // namespace soul