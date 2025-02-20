#include <gtest/gtest.h>
#include "fireball.h"
#include "player.h"
#include "core/vector2.h"
#include <memory>

using namespace soul;

class FireballTest : public ::testing::Test {
protected:
    FireballSystem fireballSystem;
    std::shared_ptr<soul::Player> player;

    void SetUp() override {
        player = std::make_shared<soul::Player>("PlayerSprite");
        fireballSystem.initFireballs(player.get());
    }
};

TEST_F(FireballTest, FireballInitialization) {
    Fireball fireball(fireballSystem, "fireball", 100.0f, 200.0f, 1.0f, 250.0f, 1, 0.0f);
    EXPECT_FLOAT_EQ(fireball.getSpeedX(), 250.0f);
    EXPECT_FLOAT_EQ(fireball.getLifetime(), 1.0f);
}

TEST_F(FireballTest, FireballSystemInit) {
    EXPECT_FALSE(fireballSystem.getFireballs().empty());
    EXPECT_EQ(fireballSystem.getFireballs().size(), fireballSystem.getFireballCount());
}