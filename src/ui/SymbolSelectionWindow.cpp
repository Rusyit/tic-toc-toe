#include "SymbolSelectionWindow.hpp"
#include "GameWindow.hpp"

void showSymbolSelectionWindow(int gridSize, GameSession& session) {
    sf::RenderWindow symbolWindow(sf::VideoMode(400, 300), "Select symbol", sf::Style::Titlebar | sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("assets/ShareTech-Regular.ttf")) return;

    sf::Text headerText("Choose your symbol:", font, 30);
    headerText.setFillColor(sf::Color::White);
    headerText.setPosition(400/2 - headerText.getLocalBounds().width/2, 20);

    const float buttonWidth = 100.f;
    const float buttonHeight = 100.f;
    const float buttonSpacing = 40.f;

    sf::RectangleShape xButton(sf::Vector2f(buttonWidth, buttonHeight));
    xButton.setPosition((400 - buttonWidth*2 - buttonSpacing) / 2, 100);
    xButton.setFillColor(sf::Color(50, 50, 50, 200));

    sf::Text xText("X", font, 70);
    xText.setPosition(
        xButton.getPosition().x + (buttonWidth - xText.getLocalBounds().width)/2 - 2,
        xButton.getPosition().y + (buttonHeight - xText.getLocalBounds().height)/2 - 18
    );
    xText.setFillColor(sf::Color::Red);

    sf::RectangleShape oButton(sf::Vector2f(buttonWidth, buttonHeight));
    oButton.setPosition(xButton.getPosition().x + buttonWidth + buttonSpacing, 100);
    oButton.setFillColor(sf::Color(50, 50, 50, 200));

    sf::Text oText("O", font, 70);
    oText.setPosition(
        oButton.getPosition().x + (buttonWidth - oText.getLocalBounds().width)/2 - 4,
        oButton.getPosition().y + (buttonHeight - oText.getLocalBounds().height)/2 - 20
    );
    oText.setFillColor(sf::Color::Blue);

    while(symbolWindow.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(symbolWindow);

        while(symbolWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                symbolWindow.close();
            }

            //обработка выбора символа
            if(event.type == sf::Event::MouseButtonPressed) {
                if(xButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    session.playerSymbol = 'X';
                    symbolWindow.close();
                    showGameWindow(gridSize, true, session);
                }
                else if(oButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    session.playerSymbol = 'O';
                    symbolWindow.close();
                    showGameWindow(gridSize, true, session);
                }
            }
        }

        xButton.setFillColor(xButton.getGlobalBounds().contains(mousePos.x, mousePos.y)
            ? sf::Color(80, 80, 80, 200) : sf::Color(50, 50, 50, 200));
        oButton.setFillColor(oButton.getGlobalBounds().contains(mousePos.x, mousePos.y)
            ? sf::Color(80, 80, 80, 200) : sf::Color(50, 50, 50, 200));

        symbolWindow.clear(sf::Color(30, 30, 30));
        symbolWindow.draw(headerText);
        symbolWindow.draw(xButton);
        symbolWindow.draw(xText);
        symbolWindow.draw(oButton);
        symbolWindow.draw(oText);
        symbolWindow.display();
    }
}