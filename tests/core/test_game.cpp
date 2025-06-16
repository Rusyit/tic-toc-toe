#include <gtest/gtest.h>
#include "../../src/core/Game.hpp"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game3 = new Game(3);
        game4 = new Game(4);
        game5 = new Game(5);
    }

    void TearDown() override {
        delete game3;
        delete game4;
        delete game5;
    }

    Game* game3;
    Game* game4;
    Game* game5;
};

//проверяет инициализацию игры для полей разного размера
TEST_F(GameTest, Initialization) {
    EXPECT_EQ(game3->size, 3);
    EXPECT_EQ(game3->cellSize, 100);
    EXPECT_EQ(game3->winCondition, 3);
    EXPECT_EQ(game3->state, GameState::Playing);
    EXPECT_TRUE(game3->isXTurn);
    
    EXPECT_EQ(game4->size, 4);
    EXPECT_EQ(game4->cellSize, 80);
    EXPECT_EQ(game4->winCondition, 3);
    
    EXPECT_EQ(game5->size, 5);
    EXPECT_EQ(game5->cellSize, 64);
    EXPECT_EQ(game5->winCondition, 4);
}

//проверяет выполнение валидных и невалидных ходов
TEST_F(GameTest, MakeMove) {
    game3->makeMove(0, 0, 'X');
    EXPECT_EQ(game3->board[0][0], 'X');
    EXPECT_FALSE(game3->isXTurn);
    EXPECT_EQ(game3->state, GameState::Playing);
    
    game3->makeMove(0, 0, 'O');
    EXPECT_EQ(game3->board[0][0], 'X');
    
    game3->makeMove(-1, 0, 'O');
    game3->makeMove(3, 0, 'O');
    game3->makeMove(0, -1, 'O');
    game3->makeMove(0, 3, 'O');
}

//проверяет победу по горизонтали
TEST_F(GameTest, CheckWinHorizontal) {
    game3->makeMove(0, 0, 'X');
    game3->makeMove(1, 0, 'O');
    game3->makeMove(0, 1, 'X');
    game3->makeMove(1, 1, 'O');
    game3->makeMove(0, 2, 'X');
    
    EXPECT_EQ(game3->state, GameState::XWon);
    EXPECT_EQ(game3->winStart, std::make_pair(0, 0));
    EXPECT_EQ(game3->winEnd, std::make_pair(0, 2));
}

//проверяет победу по вертикали
TEST_F(GameTest, CheckWinVertical) {
    game3->makeMove(0, 0, 'X');
    game3->makeMove(0, 1, 'O');
    game3->makeMove(1, 0, 'X');
    game3->makeMove(1, 1, 'O');
    game3->makeMove(2, 2, 'X');
    game3->makeMove(2, 1, 'O');
    
    EXPECT_EQ(game3->state, GameState::OWon);
    EXPECT_EQ(game3->winStart, std::make_pair(0, 1));
    EXPECT_EQ(game3->winEnd, std::make_pair(2, 1));
}

//проверяет победу по диагонали
TEST_F(GameTest, CheckWinDiagonal) {
    game3->makeMove(0, 0, 'X');
    game3->makeMove(0, 1, 'O');
    game3->makeMove(1, 1, 'X');
    game3->makeMove(0, 2, 'O');
    game3->makeMove(2, 2, 'X');
    
    EXPECT_EQ(game3->state, GameState::XWon);
    EXPECT_EQ(game3->winStart, std::make_pair(0, 0));
    EXPECT_EQ(game3->winEnd, std::make_pair(2, 2));
}

//проверяет победу по обратной диагонали
TEST_F(GameTest, CheckWinAntiDiagonal) {
    game3->makeMove(0, 0, 'X');
    game3->makeMove(0, 2, 'O');
    game3->makeMove(1, 0, 'X');
    game3->makeMove(1, 1, 'O');
    game3->makeMove(2, 1, 'X');
    game3->makeMove(2, 0, 'O');
    
    EXPECT_EQ(game3->state, GameState::OWon);
    EXPECT_EQ(game3->winStart, std::make_pair(0, 2));
    EXPECT_EQ(game3->winEnd, std::make_pair(2, 0));
}

//проверяет ничью
TEST_F(GameTest, CheckDraw) {
    game3->makeMove(0, 0, 'X');
    game3->makeMove(0, 1, 'O');
    game3->makeMove(0, 2, 'X');
    game3->makeMove(1, 0, 'O');
    game3->makeMove(1, 1, 'X');
    game3->makeMove(1, 2, 'O');
    game3->makeMove(2, 0, 'O');
    game3->makeMove(2, 1, 'X');
    game3->makeMove(2, 2, 'O');
    
    EXPECT_EQ(game3->state, GameState::Draw);
}

//проверяет сброс игры
TEST_F(GameTest, ResetGame) {
    game3->makeMove(0, 0, 'X');
    game3->makeMove(0, 1, 'O');
    game3->reset();
    
    EXPECT_EQ(game3->board[0][0], ' ');
    EXPECT_EQ(game3->board[0][1], ' ');
    EXPECT_TRUE(game3->isXTurn);
    EXPECT_EQ(game3->state, GameState::Playing);
}

//проверяет условие победы для доски 4x4
TEST_F(GameTest, WinCondition4x4) {
    game4->makeMove(0, 0, 'X');
    game4->makeMove(1, 0, 'O');
    game4->makeMove(0, 1, 'X');
    game4->makeMove(1, 1, 'O');
    game4->makeMove(0, 2, 'X');
    
    EXPECT_EQ(game4->state, GameState::XWon);
}

//проверяет условие победы для доски 5x5
TEST_F(GameTest, WinCondition5x5) {
    game5->makeMove(0, 0, 'X');
    game5->makeMove(1, 0, 'O');
    game5->makeMove(0, 1, 'X');
    game5->makeMove(1, 1, 'O');
    game5->makeMove(0, 2, 'X');
    game5->makeMove(1, 2, 'O');
    game5->makeMove(0, 3, 'X');
    
    EXPECT_EQ(game5->state, GameState::XWon);
}