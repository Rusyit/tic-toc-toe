#pragma once
#include "Game.hpp"

//класс для бота
class Bot {
public:
    //проверка может ли игрок выиграть следующим ходом
    bool canWinNextMove(Game& game, char player, int& winRow, int& winCol);
    
    //поиск оптимального хода для бота
    void findBestMove(Game& game, char botSymbol, char playerSymbol, int& row, int& col);
    
    //выполнение хода бота
    void makeBotMove(Game& game, char botSymbol, char playerSymbol);
};