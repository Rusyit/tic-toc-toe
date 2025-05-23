#include <SFML/Graphics.hpp>
#include <locale>
#include <vector>
#include <cmath>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 600;

enum class GameState {
    Playing,
    XWon,
    OWon,
    Draw
};

struct Game {
    int size;
    int cellSize;
    int winCondition;
    std::vector<std::vector<char>> board;
    bool isXTurn;
    GameState state;
    std::pair<int, int> winStart;
    std::pair<int, int> winEnd;

    Game(int gridSize = 3) : size(gridSize) {
        if(size == 3) cellSize = 100;
        else if(size == 4) cellSize = 80;
        else if(size == 5) cellSize = 64;
        
        board.resize(size, std::vector<char>(size, ' '));
        winCondition = (size == 5) ? 4 : 3;
        isXTurn = true;
        state = GameState::Playing;
        winStart = {-1, -1};
        winEnd = {-1, -1};
    }
};

void resetGame(Game& game) {
    game.board.assign(game.size, std::vector<char>(game.size, ' '));
    game.isXTurn = true;
    game.state = GameState::Playing;
    game.winStart = {-1, -1};
    game.winEnd = {-1, -1};
}

void drawWinLine(sf::RenderWindow& window, const Game& game, int fieldOffsetX, int fieldOffsetY) {
    if (game.winStart.first == -1) return;

    sf::Vector2f start(
        fieldOffsetX + game.winStart.second * game.cellSize + game.cellSize/2,
        fieldOffsetY + game.winStart.first * game.cellSize + game.cellSize/2
    );
    sf::Vector2f end(
        fieldOffsetX + game.winEnd.second * game.cellSize + game.cellSize/2,
        fieldOffsetY + game.winEnd.first * game.cellSize + game.cellSize/2
    );

    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;

    sf::RectangleShape line(sf::Vector2f(length, 5));
    line.setPosition(start);
    line.setRotation(angle);
    line.setOrigin(0, line.getSize().y / 2);
    line.setFillColor(sf::Color::White);
    
    window.draw(line);
}

void drawBoard(sf::RenderWindow& window, const Game& game, const sf::Font& font) {
    const int fieldSize = game.size * game.cellSize;
    const int fieldOffsetX = (WINDOW_WIDTH - fieldSize) / 2;
    const int fieldOffsetY = 100;

    sf::RectangleShape line(sf::Vector2f(fieldSize, 2));
    line.setFillColor(sf::Color::White);
    
    for (int i = 1; i < game.size; ++i) {
        line.setPosition(fieldOffsetX, fieldOffsetY + i * game.cellSize);
        window.draw(line);
        line.setRotation(90);
        line.setPosition(fieldOffsetX + i * game.cellSize, fieldOffsetY);
        window.draw(line);
        line.setRotation(0);
    }

    for (int i = 0; i < game.size; ++i) {
        for (int j = 0; j < game.size; ++j) {
            sf::Vector2f position(
                fieldOffsetX + j * game.cellSize + game.cellSize/2,
                fieldOffsetY + i * game.cellSize + game.cellSize/2
            );

            if (game.board[i][j] == 'X') {
                int fontSize = static_cast<int>(game.cellSize * 0.8f);
                sf::Text xText("X", font, fontSize);
                
                sf::FloatRect textBounds = xText.getLocalBounds();
                xText.setOrigin(
                    textBounds.left + textBounds.width/2 + 2,
                    textBounds.top + textBounds.height/2 - 1
                );
                xText.setPosition(position);
                xText.setFillColor(sf::Color::Red);
                window.draw(xText);
            }
            else if (game.board[i][j] == 'O') {
                int fontSize = static_cast<int>(game.cellSize * 0.75f);
                sf::Text oText("O", font, fontSize);
                
                sf::FloatRect textBounds = oText.getLocalBounds();
                oText.setOrigin(
                    textBounds.left + textBounds.width/2 + 2,
                    textBounds.top + textBounds.height/2
                );
                oText.setPosition(position);
                oText.setFillColor(sf::Color::Blue);
                window.draw(oText);
            }
        }
    }

    if (game.state == GameState::XWon || game.state == GameState::OWon) {
        drawWinLine(window, game, fieldOffsetX, fieldOffsetY);
    }
}

GameState checkWin(Game& game) {
    const int size = game.size;
    const int win = game.winCondition;

    for(int row = 0; row < size; row++) {
        for(int col = 0; col <= size - win; col++) {
            bool winLine = true;
            for(int i = 0; i < win; i++) {
                if(game.board[row][col + i] == ' ' || 
                   game.board[row][col] != game.board[row][col + i]) {
                    winLine = false;
                    break;
                }
            }
            if(winLine) {
                game.winStart = {row, col};
                game.winEnd = {row, col + win - 1};
                return (game.board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    for(int col = 0; col < size; col++) {
        for(int row = 0; row <= size - win; row++) {
            bool winLine = true;
            for(int i = 0; i < win; i++) {
                if(game.board[row + i][col] == ' ' || 
                   game.board[row][col] != game.board[row + i][col]) {
                    winLine = false;
                    break;
                }
            }
            if(winLine) {
                game.winStart = {row, col};
                game.winEnd = {row + win - 1, col};
                return (game.board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    for(int row = 0; row <= size - win; row++) {
        for(int col = 0; col <= size - win; col++) {
            bool winLine = true;
            for(int i = 0; i < win; i++) {
                if(game.board[row + i][col + i] == ' ' || 
                   game.board[row][col] != game.board[row + i][col + i]) {
                    winLine = false;
                    break;
                }
            }
            if(winLine) {
                game.winStart = {row, col};
                game.winEnd = {row + win - 1, col + win - 1};
                return (game.board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    for(int row = 0; row <= size - win; row++) {
        for(int col = win - 1; col < size; col++) {
            bool winLine = true;
            for(int i = 0; i < win; i++) {
                if(game.board[row + i][col - i] == ' ' || 
                   game.board[row][col] != game.board[row + i][col - i]) {
                    winLine = false;
                    break;
                }
            }
            if(winLine) {
                game.winStart = {row, col};
                game.winEnd = {row + win - 1, col - win + 1};
                return (game.board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    for(const auto& row : game.board)
        for(char cell : row)
            if(cell == ' ') return GameState::Playing;

    return GameState::Draw;
}

void showGameWindow(int gridSize = 3) {
    sf::RenderWindow gameWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic-Tac-Toe", sf::Style::Titlebar | sf::Style::Close);
    
    sf::Font font;
    if (!font.loadFromFile("assets/ShareTech-Regular.ttf")) return;

    int xWins = 0;
    int oWins = 0;

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
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(30);
    statusText.setFillColor(sf::Color::White);

    GameState previousState = GameState::Playing;

    while (gameWindow.isOpen()) {
        if ((game.state == GameState::XWon || game.state == GameState::OWon) && 
            previousState == GameState::Playing) {
            if (game.state == GameState::XWon) xWins++;
            else oWins++;
        }
        previousState = game.state;

        // Новое позиционирование текста статуса
        const int textYPosition = 450; 
        statusText.setPosition(WINDOW_WIDTH / 2, textYPosition);
        statusText.setOrigin(
            statusText.getLocalBounds().width / 2,
            statusText.getLocalBounds().height / 2
        );

        winsText.setString("O Wins: " + std::to_string(oWins) + "   |   X Wins: " + std::to_string(xWins));
        winsText.setOrigin(winsText.getLocalBounds().width/2, winsText.getLocalBounds().height/2);
        winsText.setPosition(WINDOW_WIDTH/2, 30);

        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
        bool shouldClose = false;

        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
                shouldClose = true;
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                int fieldSize = game.size * game.cellSize;
                int fieldOffsetX = (WINDOW_WIDTH - fieldSize) / 2;
                int fieldOffsetY = 100;
                
                if (mousePos.x >= fieldOffsetX && mousePos.x <= fieldOffsetX + fieldSize &&
                    mousePos.y >= fieldOffsetY && mousePos.y <= fieldOffsetY + fieldSize) 
                {
                    if (game.state == GameState::Playing) {
                        int row = (mousePos.y - fieldOffsetY) / game.cellSize;
                        int col = (mousePos.x - fieldOffsetX) / game.cellSize;
                        
                        if (row >= 0 && row < game.size && col >= 0 && col < game.size &&
                            game.board[row][col] == ' ') 
                        {
                            game.board[row][col] = game.isXTurn ? 'X' : 'O';
                            game.isXTurn = !game.isXTurn;
                            game.state = checkWin(game);
                        }
                    }
                }

                if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    resetGame(game);
                }
                else if (mainMenuButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    gameWindow.close();
                    shouldClose = true;
                }
            }
        }

        if (shouldClose) break;

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

        switch (game.state) {
            case GameState::Playing:
                statusText.setString(game.isXTurn ? "X's Turn" : "O's Turn");
                break;
            case GameState::XWon:
                statusText.setString("X Wins!");
                break;
            case GameState::OWon:
                statusText.setString("O Wins!");
                break;
            case GameState::Draw:
                statusText.setString("Draw!");
                break;
        }

        gameWindow.clear(sf::Color(40, 40, 40));
        gameWindow.draw(winsText);
        drawBoard(gameWindow, game, font);
        gameWindow.draw(statusText);
        if(restartButton.getTexture()) gameWindow.draw(restartButton);
        if(mainMenuButton.getTexture()) gameWindow.draw(mainMenuButton);
        gameWindow.display();
    }
}

void showSizeSelectionWindow() {
    sf::RenderWindow window(sf::VideoMode(400, 350), "Select Size", sf::Style::Titlebar | sf::Style::Close);
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
            btn.getPosition().y + (buttonHeight - text.getLocalBounds().height)/2 - 5
        );
        texts.push_back(text);
    }

    while(window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed) {
                for(int i = 0; i < 3; i++) {
                    if(buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        window.close();
                        showGameWindow(3 + i);
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

int main() {
    std::locale::global(std::locale(""));
    
    while(true) {
        sf::RenderWindow mainWindow(sf::VideoMode(600, 750), "TTT", sf::Style::Titlebar | sf::Style::Close);
        
        // Фон
        sf::Texture backgroundTexture;
        if(!backgroundTexture.loadFromFile("assets/Fon1.jpeg")) break;
        sf::Sprite background(backgroundTexture);
        background.setScale(
            static_cast<float>(mainWindow.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(mainWindow.getSize().y) / backgroundTexture.getSize().y
        );

        const float buttonWidth = 280.f;
        const float buttonHeight = 88.f;

        sf::RectangleShape btn1(sf::Vector2f(buttonWidth, buttonHeight));
        btn1.setPosition(161, 334);
        btn1.setFillColor(sf::Color(50, 50, 50, 200));

        sf::RectangleShape btn2(sf::Vector2f(buttonWidth, buttonHeight));
        btn2.setPosition(161, 474);
        btn2.setFillColor(sf::Color(50, 50, 50, 50));

        bool gameStarted = false;
        while(mainWindow.isOpen()) {
            sf::Event event;
            sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);

            while(mainWindow.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    mainWindow.close();
                    return 0;
                }

                if(event.type == sf::Event::MouseButtonPressed) {
                    if(btn1.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        mainWindow.close();
                        showGameWindow(3);
                        gameStarted = true;
                    }
                    else if(btn2.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        mainWindow.close();
                        showSizeSelectionWindow();
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