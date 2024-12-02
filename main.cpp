#include "Utility.h"
#include "StartScreen.h"
#include "EndScreen.h"
#include "Database.h"
#include "UFO.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mysql.h>
#include <iostream>
#include <vector>
#include <string>

// 게임 진해 상태
enum class GameState { StartScreen, EnterName, MainGame, EndScreen };

using namespace sf;
using namespace std;

int main() {
	RenderWindow window(VideoMode(1280, 900), L"외계인을 집에 보내줘~~~");
	Image icon;
	icon.loadFromFile("img/icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	GameState currentState = GameState::StartScreen;
	string playerName = "";
	int totalScore = 0;
	Font font;
	font.loadFromFile("DungGeunMo.ttf");

	StartScreen startScreen;
	EndScreen endScreen;

	// db 연결
	MYSQL* dbConnection = connectToDatabase();
	if (dbConnection == nullptr) {
		throw runtime_error("Failed to connect to the database.");
	}

	while (window.isOpen()) {
		// 시작
		switch (currentState) {
		case GameState::StartScreen: {
			if (startScreen.show(window)) {
				currentState = GameState::EnterName;
			}
			else {
				window.close();
			}
			break;
		}

		// 이름 입력
		case GameState::EnterName: {

			Text prompt(L"당신의 이름은?", font, 34);
			prompt.setFillColor(Color::White);
			prompt.setPosition(521, 334);

			Texture nameinputT;
			nameinputT.loadFromFile("img/nameInputField.png");
			RectangleShape nameinputField(Vector2f(327, 79));
			nameinputField.setPosition(477, 410);
			nameinputField.setTexture(&nameinputT);

			Text nameInput(playerName, font, 28);
			nameInput.setFillColor(Color(27, 29, 76));
			nameInput.setPosition(516, 431);

			playerName = "";

			bool nameEntered = false;
			while (window.isOpen() && !nameEntered) {
				Event event;
				while (window.pollEvent(event)) {
					if (event.type == Event::Closed) {
						window.close();
					}

					if (event.type == Event::TextEntered) {
						if (event.text.unicode == '\b' && !playerName.empty()) {
							playerName.pop_back();
						}
						else if (event.text.unicode == '\r') {
							if (!playerName.empty()) {
								currentState = GameState::MainGame;
								nameEntered = true;
							}
						}
						else if (event.text.unicode < 128 && event.text.unicode != '\r') {
							if (playerName.size() < 17) {
								playerName += static_cast<char>(event.text.unicode);
							}
						}
					}
				}

				nameInput.setString(playerName);

				window.clear(Color(27, 29, 76));
				window.draw(prompt);
				window.draw(nameinputField);
				window.draw(nameInput);
				window.display();
			}
			break;
		}

		// 메인
		case GameState::MainGame: {
			vector<string> ufoTextures = {
				"img/pinkUfo.png", "img/yellowUfo.png", "img/purpleUfo.png",
				"img/greenUfo.png", "img/pinkUfo.png"
			};

			vector<string> audioFiles = {
				"music/APT.wav", "music/SoLetGo.wav", "music/NotGoingAnywhere.wav",
				"music/ImSoTired.wav", "music/LifesTooShort.wav"
			};

			vector<string> correctLyrics = {
				"don't you want me like I want you baby",
				"without you late nights don't mean a thing yeah",
				"tide will rise and fall along the bay",
				"party trying my best to meet somebody",
				"you need to get a life cause life's too short"
			};

			UFO ufo;

			int currentChapter = 0;
			bool submitted = false;
			bool showNextButton = false;
			bool songFinished = false;
			int distance = -1;

			totalScore = 0;

			SoundBuffer buffer;
			Sound sound;

			Text nextButton(L"다음으로>", font, 28);
			nextButton.setFillColor(Color::White);
			nextButton.setPosition(1057, 85);

			Text inputText("", font, 24);
			inputText.setFillColor(Color(27, 29, 76));
			inputText.setPosition(406, 447);

			Text scoreText("", font, 90);
			scoreText.setFillColor(Color::Yellow);
			scoreText.setPosition(573, 230);

			Texture inputT;
			inputT.loadFromFile("img/inputField.png");
			RectangleShape inputField(Vector2f(800, 147));
			inputField.setPosition(354, 397);
			inputField.setTexture(&inputT);

			RectangleShape progressBarBackground(Vector2f(786, 24));
			progressBarBackground.setFillColor(Color(58, 84, 198));
			progressBarBackground.setPosition(132, 87);

			RectangleShape progressBar(Vector2f(786, 24));
			progressBar.setPosition(132, 87);

			const float maxTime = 10.0f;
			Clock progressClock;
			string userInput;
			Clock clock;

			buffer.loadFromFile(audioFiles[currentChapter]);
			sound.setBuffer(buffer);
			sound.play();

			bool inMainGame = true;
			while (inMainGame && window.isOpen()) {
				Event event;
				while (window.pollEvent(event)) {
					if (event.type == Event::Closed) {
						window.close();
					}

					ufo.setTexture(ufoTextures[currentChapter]);

					// 사용자가 입력할 수 있는 상태
					if (!submitted && songFinished && event.type == Event::TextEntered) {
						if (event.text.unicode == '\b' && !userInput.empty()) {
							userInput.pop_back();
						}
						else if (event.text.unicode < 128 && event.text.unicode != '\r') {
							if (userInput.size() < 57) // 글자수를 57 글자로 제한
								userInput += static_cast<char>(event.text.unicode);
						}
					}

					// "다음으로" 버튼 클릭 처리
					if (showNextButton && event.type == Event::MouseButtonPressed) {
						if (isMouseOver(nextButton, window)) {
							currentChapter++;
							if (currentChapter < audioFiles.size()) {
								userInput.clear();
								showNextButton = false;
								submitted = false;
								songFinished = false;

								buffer.loadFromFile(audioFiles[currentChapter]);
								sound.setBuffer(buffer);
								sound.play();
								progressClock.restart();
							}
							else {
								currentState = GameState::EndScreen;
								inMainGame = false; 
							}
						}
					}
				}
				if (!inMainGame) break;

				if (showNextButton) {
					nextButton.setFillColor(isMouseOver(nextButton, window) ? Color::Yellow : Color::White);
				}

				// 노래 상태 확인 및 입력 가능 상태 전환
				if (!songFinished && sound.getStatus() == Sound::Stopped) {
					songFinished = true; // 노래가 끝났음
					progressClock.restart(); // 입력 시간을 시작
				}

				// 입력 가능한 경우 진행 시간 업데이트
				if (songFinished) {
					float elapsedTime = progressClock.getElapsedTime().asSeconds();
					float remainingTime = max(0.0f, maxTime - elapsedTime);
					float progressRatio = remainingTime / maxTime;

					progressBar.setSize(Vector2f(786 * progressRatio, 24));
					if (progressRatio > 0.5f) {
						progressBar.setFillColor(Color(50, 205, 50));
					}
					else if (progressRatio > 0.2f) {
						progressBar.setFillColor(Color(255, 165, 0));
					}
					else {
						progressBar.setFillColor(Color(255, 69, 0));
					}

					// 10초가 지나면 자동 제출
					if (elapsedTime >= maxTime && !submitted) {
						submitted = true;
						distance = calculateLevenshteinDistance(userInput, correctLyrics[currentChapter]);
						showNextButton = true;

						// 점수 계산 및 누적
						int maxLen = max(correctLyrics[currentChapter].size(), userInput.size());
						int similarityScore = max(0, 100 - (distance * 100 / maxLen));
						totalScore += similarityScore; // 총 점수에 현재 점수를 추가
						scoreText.setString(to_string(similarityScore) + L"점");
					}
				}

				inputText.setString(userInput);

				float deltaTime = clock.restart().asSeconds();
				ufo.update(deltaTime);

				window.clear(Color(27, 29, 76));
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
			break;
		}

		// 엔딩
		case GameState::EndScreen: {
			saveScore(dbConnection, playerName, totalScore); // 점수 저장
			auto topScores = getTopScores(dbConnection); // 상위 순위 불러오기
			endScreen.setFinalScore(playerName, totalScore);
			endScreen.setTopScores(topScores);
			if (endScreen.show(window)) {
				currentState = GameState::StartScreen; // 시작화면으로 버튼 클릭 시
			}
			else {
				window.close();
			}
			break;
		}
		}
	}
	mysql_close(dbConnection); // db 연결 끊기
	return 0;
}