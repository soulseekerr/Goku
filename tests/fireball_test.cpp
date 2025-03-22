#include <gtest/gtest.h>
#include "fireball.h"
#include "player.h"
#include "vector2.h"
#include <memory>

using namespace soul;

class FireballTest : public ::testing::Test {
public:
    FireballSystem fireballSystem;
    std::unique_ptr<soul::Player> player = std::make_unique<soul::Player>("PlayerSprite");
};

TEST_F(FireballTest, FireballInitialization) {
    constexpr auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/fireballs.png";
    fireballSystem.initFireballs(player.get(), filePath, 300.0, 2.0);
    Fireball fireball(fireballSystem, "fireball", 100.0f, 200.0f, 1.0f, 250.0f, 1, 0.0f);
    EXPECT_FLOAT_EQ(fireball.getSpeedX(), 250.0f);
    EXPECT_FLOAT_EQ(fireball.getLifetime(), 1.0f);
}

TEST_F(FireballTest, FireballSystemInit) {
    constexpr auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/fireballs.png";
    fireballSystem.initFireballs(player.get(), filePath, 500.0, 1.0);
    EXPECT_FALSE(fireballSystem.getFireballs().empty());
    EXPECT_EQ(fireballSystem.getFireballs().size(), fireballSystem.getFireballCount());
}