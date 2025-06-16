#include <gtest/gtest.h>
#include "../../src/core/Bot.hpp"
#include "../../src/core/Game.hpp"
#include <chrono>

class BotTest : public ::testing::Test {
protected:
    void SetUp() override {
        game3 = new Game(3);
        game5 = new Game(5);
        bot = new Bot();
    }

    void TearDown() override {
        delete game3;
        delete game5;
        delete bot;
    }

    Game* game3;
    Game* game5;
    Bot* bot;
};

//проверяет способность бота завершить игру выигрышным ходом
TEST_F(BotTest, WinImmediately) {
    game3->board = {{'X','X',' '}, {'O',' ',' '}, {' ',' ',' '}};
    bot->makeBotMove(*game3, 'X', 'O');
    EXPECT_EQ(game3->board[0][2], 'X');
    EXPECT_EQ(game3->state, GameState::XWon);
}

//проверяет способность бота блокировать выигрыш соперника
TEST_F(BotTest, BlockOpponentWin) {
    game3->board = {{'O','O',' '}, {'X',' ',' '}, {' ',' ',' '}};
    game3->isXTurn = false;
    bot->makeBotMove(*game3, 'O', 'X');
    EXPECT_EQ(game3->board[0][2], 'O');
}

//проверяет стратегию бота для больших досок (5x5)
TEST_F(BotTest, LargeBoardStrategy) {
    game5->makeMove(0, 0, 'X');
    game5->makeMove(1, 1, 'O');
    
    int row, col;
    bot->findBestMove(*game5, 'X', 'O', row, col);
    EXPECT_TRUE(row >= 0 && row < 5);
    EXPECT_TRUE(col >= 0 && col < 5);
}

//проверяет первый ход бота на пустой доске
TEST_F(BotTest, EmptyBoard) {
    int row, col;
    bot->findBestMove(*game3, 'X', 'O', row, col);
    EXPECT_EQ(row, 1);
    EXPECT_EQ(col, 1);
}

//проверяет выбор хода бота на почти заполненной доске
TEST_F(BotTest, AlmostFullBoard) {
    game3->board = {{'X','O','X'}, {'O',' ','O'}, {'X','O','X'}};
    int row, col;
    bot->findBestMove(*game3, 'O', 'X', row, col);
    EXPECT_EQ(row, 1);
    EXPECT_EQ(col, 1);
}

//проверяет производительность бота на маленькой доске (3x3)
TEST_F(BotTest, PerformanceSmallBoard) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        Game tempGame(3);
        bot->makeBotMove(tempGame, 'X', 'O');
    }
    auto duration = std::chrono::high_resolution_clock::now() - start;
    EXPECT_LT(duration, std::chrono::milliseconds(50));
}

//проверяет производительность бота на большой доске (10x10)
TEST_F(BotTest, PerformanceLargeBoard) {
    auto start = std::chrono::high_resolution_clock::now();
    Game largeGame(10);
    bot->makeBotMove(largeGame, 'X', 'O');
    auto duration = std::chrono::high_resolution_clock::now() - start;
    EXPECT_LT(duration, std::chrono::milliseconds(500));
}