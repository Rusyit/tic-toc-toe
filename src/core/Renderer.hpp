#pragma once
#include <SFML/Graphics.hpp>
#include "Game.hpp"

//класс для отрисовки игры
class Renderer {
public:
    void drawBoard(sf::RenderWindow& window, const Game& game, const sf::Font& font);
    
private:
    void drawGrid(sf::RenderWindow& window, const Game& game, int offsetX, int offsetY);
    void drawSymbols(sf::RenderWindow& window, const Game& game, const sf::Font& font, int offsetX, int offsetY);
    void drawX(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f position, int cellSize);
    void drawO(sf::RenderWindow& window, const sf::Font& font, sf::Vector2f position, int cellSize);
    void centerText(sf::Text& text, sf::Vector2f position);
    void drawWinLine(sf::RenderWindow& window, const Game& game, int offsetX, int offsetY);
};