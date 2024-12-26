
#include <gtest/gtest.h>
#include "texture2d.h"
#include "core/utils.h"
#include <iostream>


// Demonstrate some basic assertions.
TEST(TextureTest, BasicAssertions) {

    auto name = "PlayerTexture";
    auto filePath = "/Users/soulseeker/Projects/GitHub/gokugame/textures/Kid Goku.png";
    auto isSmooth = true;

    auto newTexture = std::make_shared<soul::Texture2d>(name, filePath, isSmooth);

    auto resp = newTexture->loadTexture(filePath);
    if (resp.status != soul::Status::OK) {
        std::cout << "Error: " << resp.msg << std::endl;
    }
    EXPECT_EQ(resp.status == soul::Status::OK, true);

    auto name2 = "EnemyTexture";
    auto filePath2 = "/Users/soulseeker/Projects/GitHub/gokugame/textures/enemy.png";
    auto trColor = soul::Color(153, 41, 189);  // purple
    
    auto newTexture2 = std::make_shared<soul::Texture2d>(name2, filePath2, isSmooth);

    auto resp2 = newTexture->loadTextureFromImage(filePath2, trColor);
    if (resp2.status != soul::Status::OK) {
        std::cout << "Error2: " << resp2.msg << std::endl;
    }
    EXPECT_EQ(resp2.status == soul::Status::OK, true);
}
