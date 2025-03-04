#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <chrono>
#include <unordered_set>
#include "rect.h"

namespace soul {

TEST(RectTest, Contains) {
    soul::Rect<float> rect(0, 0, 10, 10);
    EXPECT_TRUE(rect.contains(soul::Vector2<float>(5, 5)));
    EXPECT_FALSE(rect.contains(soul::Vector2<float>(15, 15)));
}

TEST(RectTest, Intersects) {
    soul::Rect<float> rect1(0, 0, 10, 10);
    soul::Rect<float> rect2(5, 5, 10, 10);
    soul::Rect<float> rect3(15, 15, 5, 5);
    EXPECT_TRUE(rect1.intersects(rect2));
    EXPECT_FALSE(rect1.intersects(rect3));
}

TEST(CollisionTest, AABBIntersection) {
    soul::Rect<float> rect1(0, 0, 10, 10);
    soul::Rect<float> rect2(5, 5, 10, 10);
    soul::Rect<float> rect3(15, 15, 5, 5);
    EXPECT_TRUE(soul::Collision::isAABBIntersection(rect1, rect2));
    EXPECT_FALSE(soul::Collision::isAABBIntersection(rect1, rect3));
}

} // namespace soul
