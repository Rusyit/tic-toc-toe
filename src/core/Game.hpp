#pragma once
#include <vector>
#include <utility>

enum class GameState {
    Playing,
    XWon,
    OWon,
    Draw
};

//класс игры, содержит состояние доски и логику
class Game {
public:
    int size;
    int cellSize;
    int winCondition;
    std::vector<std::vector<char>> board;
    bool isXTurn;
    GameState state;
    std::pair<int, int> winStart;
    std::pair<int, int> winEnd;

    Game(int gridSize = 3);
    void reset();
    GameState checkWin();
    void makeMove(int row, int col, char symbol);
};