
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
        sprite->loadTexture(filePath, soul::Vector2f(1.0f, 1.0f), isSmooth);
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

    std::cout << "Textures: " << mgr.getCountTextures() << std::endl;
    EXPECT_EQ(mgr.getCountTextures(), 2);

    std::shared_ptr<soul::Texture2d> tex1, tex2;
    auto hasError3 = false;
    try {
        tex1 = mgr.getTexture("TEX_PlayerSprite");
        tex2 = mgr.getTexture("TEX_EnemySprite");
    }
    catch(std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl;
        hasError3 = true;
    }

    EXPECT_EQ(hasError3, false);
    if (!hasError3) {
        std::cout << "tex1: " << tex1->tag() << tex1->ID() << std::endl;
        std::cout << "tex2: " << tex2->tag() << tex2->ID() << std::endl;
        EXPECT_EQ(tex1->tag(), "TEX_PlayerSprite");
        EXPECT_EQ(tex2->tag(), "TEX_EnemySprite");
    }
}
