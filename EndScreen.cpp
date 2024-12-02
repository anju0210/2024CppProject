#include "EndScreen.h"
#include "Utility.h"
#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

// ���� ���� ����
void EndScreen::setFinalScore(string name, int score) {
    finalScore = score;
    playerName = name;
}

void EndScreen::setTopScores(const std::vector<std::pair<std::string, int>>& scores) {
    topScores = scores;
}

bool EndScreen::show(sf::RenderWindow& window) {
    sf::Font font;
    font.loadFromFile("DungGeunMo.ttf");

    Text title(playerName + L"���� ����", font, 34);
    title.setFillColor(Color::White);
    title.setPosition(529, 106);

    sf::Text scoreText(to_string(finalScore), font, 90);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(572, 179);

    sf::Text rankingTitle(L"����", font, 24);
    rankingTitle.setFillColor(sf::Color::White);
    rankingTitle.setPosition(477, 328);

    // ��ŷ ��������
    std::vector<sf::Text> rankingTexts;
    for (size_t i = 0; i < topScores.size(); ++i) {
        const std::string& name = topScores[i].first;
        int score = topScores[i].second;

        sf::Text ranking(std::to_string(i + 1) + ". " + name + " - " + std::to_string(score), font, 42);
        ranking.setFillColor(sf::Color::White);
        ranking.setPosition(477, 391 + i * 57);
        rankingTexts.push_back(ranking);
    }

    sf::Text restartButton(L"����ȭ������>", font, 28);
    restartButton.setFillColor(sf::Color::White);
    restartButton.setPosition(1001, 85);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed && isMouseOver(restartButton, window)) {
                return true;
            }
            if (isMouseOver(restartButton, window)) {
                restartButton.setFillColor(Color::Yellow);
            }
            else
                restartButton.setFillColor(Color::White);
        }

        window.clear(Color(27, 29, 76));
        window.draw(title);
        window.draw(scoreText);
        window.draw(rankingTitle);
        for (const auto& ranking : rankingTexts) {
            window.draw(ranking);
        }
        window.draw(restartButton);
        window.display();
    }
    return false;
}
