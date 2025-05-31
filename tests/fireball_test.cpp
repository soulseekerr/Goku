#include <gtest/gtest.h>
#include "fireball.h"
#include "player.h"
#include "vector2.h"
#include <memory>

using namespace soul;

class FireballTest : public ::testing::Test {
    protected:
        static constexpr auto filePath = "./data/player_goku.json";
        FireballSystem fireballSystem;
        std::unique_ptr<Player> player;
};

TEST_F(FireballTest, FireballInitialization) {
    try {
        fireballSystem.initFireballs(player.get(), filePath, 300.0, 2.0);
    } catch (const std::runtime_error& e) {
        FAIL() << "Failed to initialize fireballs: " << e.what();
    }
    Fireball fireball(fireballSystem, "fireball", 100.0f, 200.0f, 1.0f, 250.0f, 1, 0.0f);
    EXPECT_FLOAT_EQ(fireball.getSpeedX(), 250.0f);
    EXPECT_FLOAT_EQ(fireball.getLifetime(), 1.0f);
}

TEST_F(FireballTest, FireballSystemInit) {
    try {
        fireballSystem.initFireballs(player.get(), filePath, 300.0, 2.0);
    } catch (const std::runtime_error& e) {
        FAIL() << "Failed to initialize fireballs: " << e.what();
    }
    EXPECT_FALSE(fireballSystem.getFireballs().empty());
    EXPECT_EQ(fireballSystem.getFireballs().size(), fireballSystem.getFireballCount());
}