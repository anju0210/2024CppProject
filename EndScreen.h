#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;

class EndScreen {
private:
    int finalScore;
    string playerName;
    std::vector<std::pair<std::string, int>> topScores;

public:
    void setFinalScore(string name, int score);
    void setTopScores(const std::vector<std::pair<std::string, int>>& scores);
    bool show(sf::RenderWindow& window);
};