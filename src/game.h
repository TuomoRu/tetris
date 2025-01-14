#pragma once
#include "grid.h"
#include "blocks.cpp"

class Game {
public:
    Game();
    ~Game();
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    bool game_over;
    int score;
    Music music;

private:
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    void MoveBlockLeft();
    void MoveBlockRight();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int lines_cleared, int move_down_points);
    Grid grid;
    std::vector<Block> blocks;
    Block current_block;
    Block next_block;
    Sound rotate_sound;
    Sound clear_sound;
};