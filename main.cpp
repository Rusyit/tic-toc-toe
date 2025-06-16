#include <SFML/Graphics.hpp>
#include <locale>
#include <cstdlib>
#include <ctime>
#include "src/ui/GameWindow.hpp"
#include "src/ui/SymbolSelectionWindow.hpp"
#include "src/ui/SizeSelectionWindow.hpp"
#include "src/core/GameSession.hpp"

int main() {
    std::locale::global(std::locale(""));
    srand(static_cast<unsigned>(time(nullptr)));
    
    //главный игровой цикл
    while(true) {
        sf::RenderWindow mainWindow(sf::VideoMode(600, 750), "Tic Tac Toe", sf::Style::Titlebar | sf::Style::Close);
        
        sf::Texture backgroundTexture;
        if(!backgroundTexture.loadFromFile("assets/Fon1.jpeg")) break;
        sf::Sprite background(backgroundTexture);
        background.setScale(
            static_cast<float>(mainWindow.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(mainWindow.getSize().y) / backgroundTexture.getSize().y
        );

        const float buttonWidth = 280.f;
        const float buttonHeight = 88.f;

        //создание кнопок главного меню
        sf::RectangleShape btn1(sf::Vector2f(buttonWidth, buttonHeight));
        btn1.setPosition(161, 334);
        btn1.setFillColor(sf::Color(50, 50, 50, 50));

        sf::RectangleShape btn2(sf::Vector2f(buttonWidth, buttonHeight));
        btn2.setPosition(161, 474);
        btn2.setFillColor(sf::Color(50, 50, 50, 50));

        bool gameStarted = false;
        GameSession session;
        
        while(mainWindow.isOpen()) {
            sf::Event event;
            sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);

            while(mainWindow.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    mainWindow.close();
                    return 0;
                }

                //обработка выбора режима игры
                if(event.type == sf::Event::MouseButtonPressed) {
                    //кнопка игры с ботом
                    if(btn1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        mainWindow.close();
                        showSizeSelectionWindow(true, session);
                        gameStarted = true;
                    }
                    //кнопка игры вдвоем
                    else if(btn2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        mainWindow.close();
                        showSizeSelectionWindow(false, session);
                        gameStarted = true;
                    }
                }
            }

            if(gameStarted) break;

            btn1.setFillColor(btn1.getGlobalBounds().contains(mousePos.x, mousePos.y) 
                ? sf::Color(80, 80, 80, 0) : sf::Color(50, 50, 50, 80));
            
            btn2.setFillColor(btn2.getGlobalBounds().contains(mousePos.x, mousePos.y) 
                ? sf::Color(80, 80, 80, 0) : sf::Color(50, 50, 50, 80));

            mainWindow.clear();
            mainWindow.draw(background);
            mainWindow.draw(btn1);
            mainWindow.draw(btn2);
            mainWindow.display();
        }
        
        if(!gameStarted) break;
    }

    return 0;
}