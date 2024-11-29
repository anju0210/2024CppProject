#include "UFO.h"

void UFO::update(float deltaTime) {
    time = fmod(time + deltaTime, 2 * 3.14159f); // 주기적으로 초기화
    float offset = amplitude * sin(speed * time);
    sprite.setPosition(93, yPosition + offset);
}

void UFO::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void UFO::setTexture(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load UFO texture: " << texturePath << "\n";
    }
    sprite.setTexture(texture);
}
