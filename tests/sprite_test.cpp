
#include <gtest/gtest.h>
#include "sprite2d.h"
#include "assetmanager.h"
#include <iostream>


// Demonstrate some basic assertions.
TEST(SpriteTest, BasicAssertions) {

    std::shared_ptr<soul::Sprite2d> sprite;

    auto hasError = false;
    
    auto name = "PlayerSprite";
    auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";
    auto isSmooth = true;

    sprite = std::make_shared<soul::Sprite2d>(name);

    try {
        sprite->loadTexture(
            filePath, 
            soul::Vector2f(1.0f, 1.0f), 
            isSmooth);
    }
    catch(soul::TextureException& e) {
       std::cout << "Error: " << e.what() << std::endl;
       hasError = true;
    }

    EXPECT_EQ(hasError, false);

    auto name2 = "EnemySprite";
    auto filePath2 = "/Users/soulseeker/Projects/GitHub/gokugame/textures/enemy.png";
    auto isSmooth2 = true;
    auto trColor = soul::Color(153, 41, 189);  // purple

    auto sprite2 = std::make_shared<soul::Sprite2d>(name2);

    auto hasError2 = false;
    try {
        sprite2->loadTextureImageFilter(
            filePath2, 
            soul::Vector2f(1.0f, 1.0f), 
            isSmooth2,
            trColor);
    }
    catch(soul::TextureException& e) {
       std::cout << "Error: " << e.what() << std::endl;
       hasError2 = true;
    }

    EXPECT_EQ(hasError2, false);

    auto& mgr = soul::AssetManager::getInstance();

    EXPECT_EQ(mgr.getCountTextures(), 2);
    auto& t = mgr.getTexture("TEX_PlayerSprite");
    auto& t2 = mgr.getTexture("TEX_EnemySprite");

    EXPECT_NE(t, nullptr);
    EXPECT_NE(t2, nullptr);
}
