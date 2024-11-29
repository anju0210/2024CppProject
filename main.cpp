#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UFO.h"
#include "Utility.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace sf;

bool isMouseOver(const sf::Text& text, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    return text.getGlobalBounds().contains(mousePosF);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 900), "UFO~~~~~~~`");
    window.setFramerateLimit(60);

    UFO ufo;
    std::vector<std::string> ufoTextures = {
        "img/pinkUfo.png", "img/yellowUfo.png", "img/purpleUfo.png",
        "img/pinkUfo.png", "img/greenUfo.png"
    };

    std::vector<std::string> audioFiles = {
        "example.wav", "example.wav", "example.wav", "example.wav", "example.wav"
    };
    std::vector<std::string> correctLyrics = {
        "example", "example", "example",
        "example", "example"
    };

    int currentChapter = 0;
    bool submitted = false;
    bool showNextButton = false;
    int distance = -1;

    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile(audioFiles[currentChapter]);
    sound.setBuffer(buffer);
    sound.play();


    sf::Font font;
    font.loadFromFile("DungGeunMo.ttf");

    sf::Text nextButton(L"다음으로>", font, 28);
    nextButton.setFillColor(sf::Color::White);
    nextButton.setPosition(1057, 85);

    sf::Text inputText("", font, 24);
    inputText.setFillColor(sf::Color(27, 29, 76));
    inputText.setPosition(406, 447);

    sf::Text scoreText("", font, 90);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(573, 230);

    sf::Texture inputT;
    inputT.loadFromFile("img/inputField.png");
    sf::RectangleShape inputField(sf::Vector2f(800, 147));
    inputField.setPosition(354, 397);
    inputField.setTexture(&inputT);

    // 진행 바
    sf::RectangleShape progressBarBackground(sf::Vector2f(786, 24));
    progressBarBackground.setFillColor(sf::Color(58, 84, 198));
    progressBarBackground.setPosition(132, 87);

    sf::RectangleShape progressBar(sf::Vector2f(786, 24));
    progressBar.setPosition(132, 87);

    const float maxTime = 10.0f;
    sf::Clock progressClock;

    std::string userInput;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            ufo.setTexture(ufoTextures[currentChapter]);

            // 사용자 입력 처리
            if (!submitted && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.empty()) {
                    userInput.pop_back();
                }
                else if (event.text.unicode < 128 && event.text.unicode != '\r') {
                    if (userInput.size() < 57)
                        userInput += static_cast<char>(event.text.unicode);
                }
            }

            // "다음으로" 버튼 클릭
            if (showNextButton && event.type == sf::Event::MouseButtonPressed) {
                if (isMouseOver(nextButton, window)) {
                    currentChapter++;
                    if (currentChapter < audioFiles.size()) {
                        userInput.clear();
                        showNextButton = false;
                        submitted = false;
                        buffer.loadFromFile(audioFiles[currentChapter]);
                        sound.setBuffer(buffer);
                        sound.play();
                        progressClock.restart();
                    }
                    else {
                        std::cout << "All chapters completed!\n";
                        window.close();
                    }
                }
            }
        }

        if (showNextButton) {
            nextButton.setFillColor(isMouseOver(nextButton, window) ? sf::Color::Yellow : sf::Color::White);
        }

        // 진행 시간 업데이트
        float elapsedTime = progressClock.getElapsedTime().asSeconds();
        float remainingTime = std::max(0.0f, maxTime - elapsedTime);
        float progressRatio = remainingTime / maxTime;

        progressBar.setSize(sf::Vector2f(786 * progressRatio, 24));
        if (progressRatio > 0.5f) {
            progressBar.setFillColor(sf::Color(50, 205, 50));
        }
        else if (progressRatio > 0.2f) {
            progressBar.setFillColor(sf::Color(255, 165, 0));
        }
        else {
            progressBar.setFillColor(sf::Color(255, 69, 0));
        }

        if (elapsedTime >= maxTime && !submitted) {
            submitted = true;
            distance = calculateLevenshteinDistance(userInput, correctLyrics[currentChapter]);
            showNextButton = true;
        }

        inputText.setString(userInput);
        if (submitted) {
            int maxLen = std::max(correctLyrics[currentChapter].size(), userInput.size());
            int similarityScore = std::max(0, 100 - (distance * 100 / maxLen));
            scoreText.setString(to_string(similarityScore) + L"점");
        }

        float deltaTime = clock.restart().asSeconds();
        ufo.update(deltaTime);

        window.clear(sf::Color(27, 29, 76));
        ufo.draw(window);
        window.draw(inputField);
        window.draw(inputText);
        window.draw(progressBarBackground);
        window.draw(progressBar);

        if (submitted) {
            window.draw(scoreText);
            if (showNextButton) {
                window.draw(nextButton);
            }
        }

        window.display();
    }

    return 0;
}