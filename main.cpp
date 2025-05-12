#include <SFML/Graphics.hpp>
#include <locale>

void showGameWindow(const std::string& mode) {
    sf::RenderWindow gameWindow(sf::VideoMode(600, 750), "Game Mode - " + mode, sf::Style::Titlebar | sf::Style::Close);
    
    //загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("assets/ShareTech-Regular.ttf")) {
        return;
    }

    //элементы интерфейса
    sf::Text gameText;
    gameText.setFont(font);
    gameText.setString(mode + " Mode");
    gameText.setCharacterSize(40);
    gameText.setFillColor(sf::Color::White);
    gameText.setPosition(150, 300);

    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
        }
        
        gameWindow.clear(sf::Color(40, 40, 40));
        gameWindow.draw(gameText);
        gameWindow.display();
    }
}

int main() {
    std::locale::global(std::locale(""));
    sf::RenderWindow window(sf::VideoMode(600, 750), "TTT", sf::Style::Titlebar | sf::Style::Close);

    //загрузка фона
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/Fon1.jpeg")) {
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );

    //загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("assets/ShareTech-Regular.ttf")) {
        return 1;
    }

    //создание кнопок
    const float buttonWidth = 280.f;
    const float buttonHeight = 88.f;

    //первая кнопка
    sf::RectangleShape buttonBg1(sf::Vector2f(buttonWidth, buttonHeight));
    buttonBg1.setFillColor(sf::Color(50, 50, 50, 200)); 
    buttonBg1.setPosition(161, 334);

    sf::Text button1;
    button1.setFont(font);
    button1.setCharacterSize(40);
    button1.setFillColor(sf::Color::White);
    button1.setPosition(
        buttonBg1.getPosition().x + (buttonWidth - button1.getGlobalBounds().width)/2,
        buttonBg1.getPosition().y + 5
    );

    //вторая кнопка
    sf::RectangleShape buttonBg2(sf::Vector2f(buttonWidth, buttonHeight));
    buttonBg2.setFillColor(sf::Color(50, 50, 50, 50));
    buttonBg2.setPosition(161, 474);

    sf::Text button2;
    button2.setFont(font);
    button2.setCharacterSize(40);
    button2.setFillColor(sf::Color::White);
    button2.setPosition(
        buttonBg2.getPosition().x + (buttonWidth - button2.getGlobalBounds().width)/2,
        buttonBg2.getPosition().y + 5
    );

    const sf::Color normalColor = sf::Color::White;
    const sf::Color hoverColor = sf::Color(200, 255, 200);

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        //обработка событий
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::Resized) {
                window.setSize(sf::Vector2u(600, 750));
            }
            
            //обработка нажатия кнопок
            if (event.type == sf::Event::MouseButtonPressed) {
                if (buttonBg1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                    showGameWindow("Computer"); 
                }
                else if (buttonBg2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                    showGameWindow("Friend"); 
                }
            }
        }

        buttonBg1.setFillColor(buttonBg1.getGlobalBounds().contains(mousePos.x, mousePos.y) 
            ? sf::Color(80, 80, 80, 0) : sf::Color(50, 50, 50, 80));
        
        buttonBg2.setFillColor(buttonBg2.getGlobalBounds().contains(mousePos.x, mousePos.y) 
            ? sf::Color(80, 80, 80, 0) : sf::Color(50, 50, 50, 80));

        window.clear();
        window.draw(backgroundSprite);
        window.draw(buttonBg1);
        window.draw(buttonBg2);
        window.draw(button1);
        window.draw(button2);
        window.display();
    }

    return 0;
}