#include "UFO.h"

// 위아래 움직임 구현
void UFO::update(float deltaTime) {
    time = fmod(time + deltaTime, 2 * 3.14159f);
    float offset = amplitude * sin(speed * time);   // 이동 거리
    sprite.setPosition(93, yPosition + offset);
}

void UFO::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void UFO::setTexture(const std::string& texturePath) {  // 챕터마다 이미지 변경
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load UFO texture: " << texturePath << "\n";
    }
    sprite.setTexture(texture);
}
