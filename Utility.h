#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
using namespace sf;

int calculateLevenshteinDistance(const std::string& s1, const std::string& s2);
bool isMouseOver(const sf::Text& text, const sf::RenderWindow& window);