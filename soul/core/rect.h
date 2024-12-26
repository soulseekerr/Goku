#pragma once

#include "core/vector2.h"

namespace soul {

template <typename T>
class Rect {
public:
    Rect() : left(0), top(0), width(0), height(0) {}
    Rect(T left_, T top_, T width_, T height_) : left(left_), top(top_), width(width_), height(height_) {}
    ~Rect() = default;

    T left, top, width, height;

    bool contains(const soul::Vector2<T>& v) {
        return v.x >= left && v.x <= left+width && v.y >= top && v.y <= top+height;
    }

    // TODO to test versus isAABBIntersection
    bool intersects(const Rect<T>& r) {
        return left < r.left + r.width && left + width > r.left && top < r.top + r.height && top + height > r.y;
    }
};

class Collision {
public:
    static bool isAABBIntersection(const soul::Rect<float>& rect1, const soul::Rect<float>& rect2) {

        // Check if there is no overlap in the horizontal axis
        if (rect1.left + rect1.width <= rect2.left || rect2.left + rect2.width <= rect1.left) {
            return false;
        }
        
        // Check if there is no overlap in the vertical axis
        if (rect1.top + rect1.height <= rect2.top || rect2.top + rect2.height <= rect1.top) {
            return false;
        }
        
        // If neither condition is true, then the rectangles are intersecting
        return true;
    }
};

} // namespace soul