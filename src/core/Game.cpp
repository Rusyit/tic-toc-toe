#include "Game.hpp"

Game::Game(int gridSize) : size(gridSize) {
    //установка размера ячейки в зависимости от размера поля
    if (size == 3) cellSize = 100;
    else if (size == 4) cellSize = 80;
    else if (size == 5) cellSize = 64;
    
    board.resize(size, std::vector<char>(size, ' '));
    //условие победы 
    winCondition = (size == 5) ? 4 : 3;
    isXTurn = true;
    state = GameState::Playing;
    winStart = {-1, -1};
    winEnd = {-1, -1};
}

void Game::reset() {
    //сброс состояния игры
    board.assign(size, std::vector<char>(size, ' '));
    isXTurn = true;
    state = GameState::Playing;
    winStart = {-1, -1};
    winEnd = {-1, -1};
}

GameState Game::checkWin() {
    const int win = winCondition;

    //проверка горизонтальных линий
    for (int row = 0; row < size; row++) {
        for (int col = 0; col <= size - win; col++) {
            bool winLine = true;
            for (int i = 0; i < win; i++) {
                if (board[row][col + i] == ' ' || 
                   board[row][col] != board[row][col + i]) {
                    winLine = false;
                    break;
                }
            }
            if (winLine) {
                winStart = {row, col};
                winEnd = {row, col + win - 1};
                return (board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    //проверка вертикальных линий
    for (int col = 0; col < size; col++) {
        for (int row = 0; row <= size - win; row++) {
            bool winLine = true;
            for (int i = 0; i < win; i++) {
                if (board[row + i][col] == ' ' || 
                   board[row][col] != board[row + i][col]) {
                    winLine = false;
                    break;
                }
            }
            if (winLine) {
                winStart = {row, col};
                winEnd = {row + win - 1, col};
                return (board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    //проверка главной диагонали 
    for (int row = 0; row <= size - win; row++) {
        for (int col = 0; col <= size - win; col++) {
            bool winLine = true;
            for (int i = 0; i < win; i++) {
                if (board[row + i][col + i] == ' ' || 
                   board[row][col] != board[row + i][col + i]) {
                    winLine = false;
                    break;
                }
            }
            if (winLine) {
                winStart = {row, col};
                winEnd = {row + win - 1, col + win - 1};
                return (board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    //проверка побочной диагонали
    for (int row = 0; row <= size - win; row++) {
        for (int col = win - 1; col < size; col++) {
            bool winLine = true;
            for (int i = 0; i < win; i++) {
                if (board[row + i][col - i] == ' ' || 
                   board[row][col] != board[row + i][col - i]) {
                    winLine = false;
                    break;
                }
            }
            if (winLine) {
                winStart = {row, col};
                winEnd = {row + win - 1, col - win + 1};
                return (board[row][col] == 'X') ? GameState::XWon : GameState::OWon;
            }
        }
    }

    //проверка на ничью
    for (const auto& row : board)
        for (char cell : row)
            if (cell == ' ') return GameState::Playing;

    return GameState::Draw;
}

void Game::makeMove(int row, int col, char symbol) {
    //валидация хода
    if (row < 0 || row >= size || col < 0 || col >= size || board[row][col] != ' ')
        return;
    
    board[row][col] = symbol;
    isXTurn = !isXTurn;
    state = checkWin();
}