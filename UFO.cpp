#include "UFO.h"

// ���Ʒ� ������ ����
void UFO::update(float deltaTime) {
    time = fmod(time + deltaTime, 2 * 3.14159f);
    float offset = amplitude * sin(speed * time);   // �̵� �Ÿ�
    sprite.setPosition(93, yPosition + offset);
}

void UFO::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void UFO::setTexture(const std::string& texturePath) {  // é�͸��� �̹��� ����
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Failed to load UFO texture: " << texturePath << "\n";
    }
    sprite.setTexture(texture);
}
