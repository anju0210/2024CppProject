#include "StartScreen.h"
#include "Utility.h"
#include <iostream>

using namespace sf;


bool StartScreen::show(RenderWindow& window) {
    Font font;
    font.loadFromFile("DungGeunMo.ttf");

    Text title(L"외계인을 집에 보내줘~~~", font, 80);
    title.setFillColor(Color::Yellow);
    title.setPosition(238, 218);

    //게임 설명 텍스트 배열
    Text subTitle[3];
    subTitle[0].setString(L"노래를 듣고 가사를 기억한 후");
    subTitle[0].setPosition(472, 364);
    subTitle[1].setString(L"제한시간 내에 정확하게 가사를 적어");
    subTitle[1].setPosition(436, 408);
    subTitle[2].setString(L"길을 잃은 외계인들이 집에 잘 찾아가도록 도와주세요!");
    subTitle[2].setPosition(334, 452);
    for (int i = 0; i < 3; i++){
        subTitle[i].setFillColor(Color::White);
        subTitle[i].setFont(font);
        subTitle[i].setCharacterSize(24);
    }
    
    Text startButton(L"시작하기>", font, 32);
    startButton.setFillColor(Color::White);
    startButton.setPosition(568, 596);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == Event::MouseButtonPressed &&
                isMouseOver(startButton, window)) {
                return true;
            }
        }

        startButton.setFillColor(isMouseOver(startButton, window) ? Color::Yellow : Color::White);

        window.clear(Color(27,29,76));
        window.draw(title);
        for(int i=0; i<3; i++)
            window.draw(subTitle[i]);
        window.draw(startButton);
        window.display();
    }
    return false;
}
