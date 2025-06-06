#include "Bot.hpp"
#include <cstdlib>
#include <vector>
#include <utility>

bool Bot::canWinNextMove(Game& game, char player, int& winRow, int& winCol) {
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            if (game.board[i][j] == ' ') {
                //временная установка символа
                game.board[i][j] = player;
                GameState tempState = game.checkWin();
                game.board[i][j] = ' ';
                
                //проверка выигрышной позиции
                if ((player == 'X' && tempState == GameState::XWon) || 
                    (player == 'O' && tempState == GameState::OWon)) {
                    winRow = i;
                    winCol = j;
                    return true;
                }
            }
        }
    }
    return false;
}

void Bot::findBestMove(Game& game, char botSymbol, char playerSymbol, int& row, int& col) {
    int winRow, winCol;
    
    //попытка выиграть самому
    if (canWinNextMove(game, botSymbol, winRow, winCol)) {
        row = winRow;
        col = winCol;
        return;
    }
    
    //блокировка выигрыша игрока
    if (canWinNextMove(game, playerSymbol, winRow, winCol)) {
        row = winRow;
        col = winCol;
        return;
    }
    
    //стратегии для больших полей 
    if (game.size > 3) {
        std::vector<std::pair<int, int>> lineExtensionMoves;
        const int win = game.winCondition - 1;
        
        //поиск возможных продолжений линий
        for (int i = 0; i < game.size; i++) {
            for (int j = 0; j <= game.size - win; j++) {
                int oCount = 0;
                int emptyCount = 0;
                int emptyRow = -1, emptyCol = -1;
                
                for (int k = 0; k < win; k++) {
                    if (game.board[i][j + k] == botSymbol) oCount++;
                    else if (game.board[i][j + k] == ' ') {
                        emptyCount++;
                        emptyRow = i;
                        emptyCol = j + k;
                    }
                }
                
                if (oCount == win - 1 && emptyCount == 1) {
                    lineExtensionMoves.push_back({emptyRow, emptyCol});
                }
            }
        }
        
        for (int j = 0; j < game.size; j++) {
            for (int i = 0; i <= game.size - win; i++) {
                int oCount = 0;
                int emptyCount = 0;
                int emptyRow = -1, emptyCol = -1;
                
                for (int k = 0; k < win; k++) {
                    if (game.board[i + k][j] == botSymbol) oCount++;
                    else if (game.board[i + k][j] == ' ') {
                        emptyCount++;
                        emptyRow = i + k;
                        emptyCol = j;
                    }
                }
                
                if (oCount == win - 1 && emptyCount == 1) {
                    lineExtensionMoves.push_back({emptyRow, emptyCol});
                }
            }
        }
        
        for (int i = 0; i <= game.size - win; i++) {
            for (int j = 0; j <= game.size - win; j++) {
                int oCount = 0;
                int emptyCount = 0;
                int emptyRow = -1, emptyCol = -1;
                
                for (int k = 0; k < win; k++) {
                    if (game.board[i + k][j + k] == botSymbol) oCount++;
                    else if (game.board[i + k][j + k] == ' ') {
                        emptyCount++;
                        emptyRow = i + k;
                        emptyCol = j + k;
                    }
                }
                
                if (oCount == win - 1 && emptyCount == 1) {
                    lineExtensionMoves.push_back({emptyRow, emptyCol});
                }
            }
        }
        
        for (int i = 0; i <= game.size - win; i++) {
            for (int j = win - 1; j < game.size; j++) {
                int oCount = 0;
                int emptyCount = 0;
                int emptyRow = -1, emptyCol = -1;
                
                for (int k = 0; k < win; k++) {
                    if (game.board[i + k][j - k] == botSymbol) oCount++;
                    else if (game.board[i + k][j - k] == ' ') {
                        emptyCount++;
                        emptyRow = i + k;
                        emptyCol = j - k;
                    }
                }
                
                if (oCount == win - 1 && emptyCount == 1) {
                    lineExtensionMoves.push_back({emptyRow, emptyCol});
                }
            }
        }
        
        //выбор случайного из возможных продолжений
        if (!lineExtensionMoves.empty()) {
            int index = rand() % lineExtensionMoves.size();
            row = lineExtensionMoves[index].first;
            col = lineExtensionMoves[index].second;
            return;
        }
        
        //ходы рядом с существующими символами бота
        std::vector<std::pair<int, int>> adjacentMoves;
        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        
        for (int i = 0; i < game.size; i++) {
            for (int j = 0; j < game.size; j++) {
                if (game.board[i][j] == botSymbol) {
                    for (int d = 0; d < 8; d++) {
                        int ni = i + dx[d];
                        int nj = j + dy[d];
                        
                        if (ni >= 0 && ni < game.size && nj >= 0 && nj < game.size && 
                            game.board[ni][nj] == ' ') {
                            adjacentMoves.push_back({ni, nj});
                        }
                    }
                }
            }
        }
        
        if (!adjacentMoves.empty()) {
            int index = rand() % adjacentMoves.size();
            row = adjacentMoves[index].first;
            col = adjacentMoves[index].second;
            return;
        }
    }
    
    //попытка занять центр
    int center = game.size / 2;
    if (game.size % 2 == 1 && game.board[center][center] == ' ') {
        row = center;
        col = center;
        return;
    }
    
    //попытка занять углы
    std::vector<std::pair<int, int>> corners = {
        {0, 0}, {0, game.size - 1},
        {game.size - 1, 0}, {game.size - 1, game.size - 1}
    };
    std::vector<std::pair<int, int>> availableCorners;
    for (const auto& corner : corners) {
        if (game.board[corner.first][corner.second] == ' ') {
            availableCorners.push_back(corner);
        }
    }
    if (!availableCorners.empty()) {
        int index = rand() % availableCorners.size();
        row = availableCorners[index].first;
        col = availableCorners[index].second;
        return;
    }
    
    //случайный ход
    std::vector<std::pair<int, int>> freeCells;
    for (int i = 0; i < game.size; i++) {
        for (int j = 0; j < game.size; j++) {
            if (game.board[i][j] == ' ') {
                freeCells.push_back({i, j});
            }
        }
    }
    if (!freeCells.empty()) {
        int index = rand() % freeCells.size();
        row = freeCells[index].first;
        col = freeCells[index].second;
    } else {
        row = -1;
        col = -1;
    }
}

void Bot::makeBotMove(Game& game, char botSymbol, char playerSymbol) {
    int row, col;
    findBestMove(game, botSymbol, playerSymbol, row, col);
    
    if (row != -1 && col != -1) {
        game.makeMove(row, col, botSymbol);
    }
}