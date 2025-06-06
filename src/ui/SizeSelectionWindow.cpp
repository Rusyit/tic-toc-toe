#include "SizeSelectionWindow.hpp"
#include "GameWindow.hpp"
#include "SymbolSelectionWindow.hpp"

void showSizeSelectionWindow(bool vsBot, GameSession& session) {
    sf::RenderWindow window(sf::VideoMode(400, 350), "Select size", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("assets/ShareTech-Regular.ttf")) return;

    sf::Text headerText("Select game field:", font, 30);
    headerText.setFillColor(sf::Color::White);
    headerText.setPosition(400/2 - headerText.getLocalBounds().width/2, 20);

    const float buttonWidth = 200.f;
    const float buttonHeight = 50.f;
    std::vector<std::string> sizes = {"3x3", "4x4", "5x5"};
    const float buttonSpacing = 20.f;

    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> texts;
    
    //создание кнопок выбора размера
    for(int i = 0; i < 3; i++) {
        sf::RectangleShape btn(sf::Vector2f(buttonWidth, buttonHeight));
        btn.setPosition(
            (400 - buttonWidth) / 2, 
            100 + i*(buttonHeight + buttonSpacing)
        );
        btn.setFillColor(sf::Color(50, 50, 50, 200));
        buttons.push_back(btn);

        sf::Text text(sizes[i], font, 30);
        text.setPosition(
            btn.getPosition().x + (buttonWidth - text.getLocalBounds().width)/2,
            btn.getPosition().y + (buttonHeight - text.getLocalBounds().height)/2 - 9
        );
        texts.push_back(text);
    }

    while(window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();

            //обработка выбора размера
            if(event.type == sf::Event::MouseButtonPressed) {
                for(int i = 0; i < 3; i++) {
                    if(buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window.close();
                        if (vsBot) {
                            showSymbolSelectionWindow(3 + i, session);
                        } else {
                            showGameWindow(3 + i, false, session);
                        }
                    }
                }
            }
        }

        for(int i = 0; i < 3; i++) {
            buttons[i].setFillColor(buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)
                ? sf::Color(80, 80, 80, 200) : sf::Color(50, 50, 50, 200));
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(headerText);
        for(auto& btn : buttons) window.draw(btn);
        for(auto& text : texts) window.draw(text);
        window.display();
    }
}