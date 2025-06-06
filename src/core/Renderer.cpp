#include "Renderer.hpp"

void Renderer::drawBoard(sf::RenderWindow& window, const Game& game, const sf::Font& font) {
    const int fieldSize = game.size * game.cellSize;
    const int fieldOffsetX = (500 - fieldSize) / 2;
    const int fieldOffsetY = 100;

    drawGrid(window, game, fieldOffsetX, fieldOffsetY);
    drawSymbols(window, game, font, fieldOffsetX, fieldOffsetY);
    
    //отрисовка линии победы 
    if (game.state == GameState::XWon || game.state == GameState::OWon) {
        drawWinLine(window, game, fieldOffsetX, fieldOffsetY);
    }
}

void Renderer::drawGrid(sf::RenderWindow& window, const Game& game, int offsetX, int offsetY) {
    const int fieldSize = game.size * game.cellSize;
    sf::RectangleShape line(sf::Vector2f(fieldSize, 2));
    line.setFillColor(sf::Color::White);
    
    //рисование линий сетки
    for (int i = 1; i < game.size; ++i) {
        line.setPosition(offsetX, offsetY + i * game.cellSize);
        window.draw(line);
        line.setRotation(90);
        line.setPosition(offsetX + i * game.cellSize, offsetY);
        window.draw(line);
        line.setRotation(0);
    }
}

void Renderer::drawSymbols(sf::RenderWindow& window, const Game& game, const sf::Font& font, int offsetX, int offsetY) {
    //отрисовка X и O на поле
    for (int i = 0; i < game.size; ++i) {
        for (int j = 0; j < game.size; ++j) {
            sf::Vector2f position(
                offsetX + j * game.cellSize + game.cellSize/2,
                offsetY + i * game.cellSize + game.cellSize/2
            );

            if (game.board[i][j] == 'X') {
                drawX(window, font, position, game.cellSize);
            }
            else if (game.board[i][j] == 'O') {
                drawO(window, font, position, game.cellSize);
            }
        }
    }
}

void Renderer::drawX(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f position, int cellSize) {
    //отрисовка символа X
    int fontSize = static_cast<int>(cellSize * 0.8f);
    sf::Text xText("X", font, fontSize);
    xText.setFillColor(sf::Color::Red);
    centerText(xText, position);
    window.draw(xText);
}

void Renderer::drawO(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f position, int cellSize) {
    //отрисовка символа O
    int fontSize = static_cast<int>(cellSize * 0.75f);
    sf::Text oText("O", font, fontSize);
    oText.setFillColor(sf::Color::Blue);
    centerText(oText, position);
    window.draw(oText);
}

void Renderer::centerText(sf::Text& text, sf::Vector2f position) {
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(
        textBounds.left + textBounds.width/2 + 2,
        textBounds.top + textBounds.height/2
    );
    text.setPosition(position);
}

void Renderer::drawWinLine(sf::RenderWindow& window, const Game& game, int offsetX, int offsetY) {
    if (game.winStart.first == -1) return;

    //расчет координат линии
    sf::Vector2f start(
        offsetX + game.winStart.second * game.cellSize + game.cellSize/2,
        offsetY + game.winStart.first * game.cellSize + game.cellSize/2
    );
    sf::Vector2f end(
        offsetX + game.winEnd.second * game.cellSize + game.cellSize/2,
        offsetY + game.winEnd.first * game.cellSize + game.cellSize/2
    );

    //расчет параметров линии
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;

    //создание и отрисовка линии
    sf::RectangleShape line(sf::Vector2f(length, 5));
    line.setPosition(start);
    line.setRotation(angle);
    line.setOrigin(0, line.getSize().y / 2);
    line.setFillColor(sf::Color::White);
    
    window.draw(line);
}