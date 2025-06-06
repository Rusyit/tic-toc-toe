#pragma once

//отслеживание статистики игры
struct GameSession {
    int xWins = 0;
    int oWins = 0;
    char playerSymbol = 'X';
};