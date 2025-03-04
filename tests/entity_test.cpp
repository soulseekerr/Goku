
#include <gtest/gtest.h>
#include "entity.h"
#include <iostream>

// Demonstrate some basic assertions.
TEST(EntityTest, BasicAssertions) {

    std::shared_ptr<soul::Entity> entity = std::make_shared<soul::Entity>("Entity");
    std::cout << "Entity1: " << entity->tag() << entity->ID() << std::endl;
    EXPECT_EQ(entity->tag(), "Entity");

    soul::Entity entity2("Entity2");
    std::cout << "Entity2: " << entity2.tag() << entity->ID() << std::endl;
    EXPECT_EQ(entity2.tag(), "Entity2");
}