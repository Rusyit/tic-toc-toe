#include "GameWindow.hpp"
#include "SymbolSelectionWindow.hpp"
#include "../core/Game.hpp"
#include "../core/Renderer.hpp"
#include "../core/Bot.hpp"
#include <thread>
#include <chrono>

//размеры окна
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 600;

void showGameWindow(int gridSize, bool vsBot, GameSession& session) {
    sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic-Tac-Toe", sf::Style::Titlebar | sf::Style::Close);
    
    sf::Font font;
    if (!font.loadFromFile("assets/ShareTech-Regular.ttf")) return;

    sf::Text winsText;
    winsText.setFont(font);
    winsText.setCharacterSize(30);
    winsText.setFillColor(sf::Color::White);

    sf::Texture restartTexture;
    sf::Sprite restartButton;
    if (restartTexture.loadFromFile("assets/res.jpg")) {
        restartButton.setTexture(restartTexture);
        const float restartScale = 0.4f;
        restartButton.setScale(restartScale, restartScale);
        const float restartX = (WINDOW_WIDTH - restartButton.getGlobalBounds().width) / 2;
        const float restartY = 530;
        restartButton.setPosition(restartX, restartY);
    }

    sf::Texture menuTexture;
    sf::Sprite mainMenuButton;
    if (menuTexture.loadFromFile("assets/mainmenu.jpg")) {
        mainMenuButton.setTexture(menuTexture);
        const float buttonScale = 0.1f;
        const float margin = 10.f;
        mainMenuButton.setScale(buttonScale, buttonScale);
        sf::FloatRect bounds = mainMenuButton.getGlobalBounds();
        mainMenuButton.setPosition(
            WINDOW_WIDTH - bounds.width - margin,
            WINDOW_HEIGHT - bounds.height - margin
        );
    }

    Game game(gridSize);
    Renderer renderer;
    Bot bot;
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::White);

    GameState previousState = GameState::Playing;
    
    bool botShouldMove = false;
    sf::Clock botTimer;
    
    char botSymbol = (session.playerSymbol == 'X') ? 'O' : 'X';
    
    //бот ходит первым, если игрок выбрал 'O'
    if (vsBot && session.playerSymbol == 'O') {
        botShouldMove = true;
        botTimer.restart();
    }

    while (gameWindow.isOpen()) {
        //обновление статистики побед
        if ((game.state == GameState::XWon || game.state == GameState::OWon) && 
            previousState == GameState::Playing) {
            if (game.state == GameState::XWon) session.xWins++;
            else session.oWins++;
        }
        previousState = game.state;

        //настройка текста статуса
        const int textYPosition = 450; 
        statusText.setPosition(WINDOW_WIDTH / 2, textYPosition);
        statusText.setOrigin(
            statusText.getLocalBounds().width / 2,
            statusText.getLocalBounds().height / 2
        );

        //обновление текста статистики
        winsText.setString("O Wins: " + std::to_string(session.oWins) + "   |   X Wins: " + std::to_string(session.xWins));
        winsText.setOrigin(winsText.getLocalBounds().width/2, winsText.getLocalBounds().height/2);
        winsText.setPosition(WINDOW_WIDTH/2, 30);

        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
        bool shouldClose = false;
        bool restartWithSymbolSelection = false;

        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                shouldClose = true;
            }
            
            //обработка кликов мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                int fieldSize = game.size * game.cellSize;
                int fieldOffsetX = (WINDOW_WIDTH - fieldSize) / 2;
                int fieldOffsetY = 100;
                
                //определение чей сейчас ход
                bool isPlayersTurn = true;
                if (vsBot) {
                    isPlayersTurn = ((session.playerSymbol == 'X' && game.isXTurn) ||
                                    (session.playerSymbol == 'O' && !game.isXTurn)) &&
                                    !botShouldMove;
                }
                
                //клик по игровому полю
                if (mousePos.x >= fieldOffsetX && mousePos.x <= fieldOffsetX + fieldSize &&
                    mousePos.y >= fieldOffsetY && mousePos.y <= fieldOffsetY + fieldSize) 
                {
                    if (game.state == GameState::Playing && 
                        (!vsBot || isPlayersTurn)) 
                    {
                        int row = (mousePos.y - fieldOffsetY) / game.cellSize;
                        int col = (mousePos.x - fieldOffsetX) / game.cellSize;
                        
                        //валидация хода
                        if (row >= 0 && row < game.size && col >= 0 && col < game.size &&
                            game.board[row][col] == ' ') 
                        {
                            if (vsBot) {
                                game.makeMove(row, col, session.playerSymbol);
                                
                                //активация хода бота
                                if (game.state == GameState::Playing) {
                                    botShouldMove = true;
                                    botTimer.restart();
                                }
                            } else {
                                char symbol = game.isXTurn ? 'X' : 'O';
                                game.makeMove(row, col, symbol);
                            }
                        }
                    }
                }

                //обработка кнопки рестарта
                if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (vsBot) {
                        restartWithSymbolSelection = true;
                        shouldClose = true;
                    } else {
                        game.reset();
                        botShouldMove = false;
                    }
                }
                //обработка кнопки главного меню
                else if (mainMenuButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    gameWindow.close();
                    shouldClose = true;
                }
            }
        }

        if (shouldClose) {
            //переход к выбору символа при рестарте
            if (restartWithSymbolSelection) {
                gameWindow.close();
                showSymbolSelectionWindow(gridSize, session);
            }
            break;
        }

        //выполнение хода бота
        if (vsBot && botShouldMove && botTimer.getElapsedTime().asSeconds() >= 1.0f) {
            bot.makeBotMove(game, botSymbol, session.playerSymbol);
            botShouldMove = false;
        }

        if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            restartButton.setColor(sf::Color(180, 180, 180));
        } else {
            restartButton.setColor(sf::Color::White);
        }

        if (mainMenuButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            mainMenuButton.setColor(sf::Color(180, 180, 180));
        } else {
            mainMenuButton.setColor(sf::Color::White);
        }

        //установка текста статуса
        switch (game.state) {
            case GameState::Playing:
                if (vsBot) {
                    if ((session.playerSymbol == 'X' && game.isXTurn) || 
                        (session.playerSymbol == 'O' && !game.isXTurn)) {
                        statusText.setString("Your turn");
                    } else {
                        statusText.setString("Bot's turn");
                    }
                } else {
                    statusText.setString(game.isXTurn ? "X's turn" : "O's turn");
                }
                break;
            case GameState::XWon:
                statusText.setString(vsBot ? (session.playerSymbol == 'X' ? "You win!" : "Bot wins!") : "X wins!");
                break;
            case GameState::OWon:
                statusText.setString(vsBot ? (session.playerSymbol == 'O' ? "You win!" : "Bot wins!") : "O wins!");
                break;
            case GameState::Draw:
                statusText.setString("Draw!");
                break;
        }

        //отрисовка кадра
        gameWindow.clear(sf::Color(40, 40, 40));
        gameWindow.draw(winsText);
        renderer.drawBoard(gameWindow, game, font);
        gameWindow.draw(statusText);
        if(restartButton.getTexture()) gameWindow.draw(restartButton);
        if(mainMenuButton.getTexture()) gameWindow.draw(mainMenuButton);
        gameWindow.display();
    }
}