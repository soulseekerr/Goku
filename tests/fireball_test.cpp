#include <gtest/gtest.h>
#include "fireball.h"
#include "player.h"
#include "core/vector2.h"
#include <memory>

using namespace soul;

class FireballTest : public ::testing::Test {
};

TEST_F(FireballTest, FireballInitialization) {
    std::unique_ptr<soul::Player> player = std::make_unique<soul::Player>("PlayerSprite");
    FireballSystem fireballSystem;
    fireballSystem.initFireballs(player.get());
    Fireball fireball(fireballSystem, "fireball", 100.0f, 200.0f, 1.0f, 250.0f, 1, 0.0f);
    EXPECT_FLOAT_EQ(fireball.getSpeedX(), 250.0f);
    EXPECT_FLOAT_EQ(fireball.getLifetime(), 1.0f);
}

TEST_F(FireballTest, FireballSystemInit) {
    std::unique_ptr<soul::Player> player = std::make_unique<soul::Player>("PlayerSprite");
    FireballSystem fireballSystem;
    fireballSystem.initFireballs(player.get());
    EXPECT_FALSE(fireballSystem.getFireballs().empty());
    EXPECT_EQ(fireballSystem.getFireballs().size(), fireballSystem.getFireballCount());
}