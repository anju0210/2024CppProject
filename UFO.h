#ifndef UFO_HPP
#define UFO_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class UFO {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float yPosition = 360;
    float speed = 3.0f;
    float amplitude = 50.0f;
    float time = 0.0f;

public:
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setTexture(const std::string& texturePath);
};

#endif // UFO_HPP
