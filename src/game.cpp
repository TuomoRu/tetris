#include "game.h"
#include <random>

Game::Game() {
    grid = Grid();
    blocks = GetAllBlocks();
    current_block = GetRandomBlock();
    next_block = GetRandomBlock();
    game_over = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("Sounds/music.mp3");
    PlayMusicStream(music);
    rotate_sound = LoadSound("Sounds/rotate.mp3");
    clear_sound = LoadSound("Sounds/clear.mp3");
}

Game::~Game() {
    UnloadSound(rotate_sound);
    UnloadSound(clear_sound);
    UnloadMusicStream(music);
    CloseAudioDevice();
}

Block Game::GetRandomBlock() {
    if (blocks.empty()) {
        blocks = GetAllBlocks();
    }
    int random_index = rand() % blocks.size();
    Block block = blocks[random_index];
    blocks.erase(blocks.begin() + random_index);
    return block;
}

std::vector<Block> Game::GetAllBlocks() {
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::Draw() {
    grid.Draw();
    current_block.Draw(11, 11);
    switch (next_block.id) {
        case 3:
            next_block.Draw(255, 290);
            break;
        case 4:
            next_block.Draw(255, 280);
            break;
        default:
            next_block.Draw(270, 270);
            break;
    }
}

void Game::HandleInput() {
    int key_pressed = GetKeyPressed();
    if (game_over && key_pressed != 0) {
        game_over = false;
        Reset();
    }
    switch (key_pressed) {
    case KEY_LEFT:
        MoveBlockLeft();
        break;
    case KEY_RIGHT:
        MoveBlockRight();
        break;
    case KEY_DOWN:
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case KEY_UP:
        RotateBlock();
        break;
    }
}

void Game::MoveBlockLeft() {
    if (!game_over) {
        current_block.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false) {
            current_block.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight() {
    if (!game_over) {
        current_block.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false) {
            current_block.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown() {
    if (!game_over) {
        current_block.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false) {
            current_block.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside() {
    std::vector<Position> tiles = current_block.GetCellPositions();
    for (Position item: tiles) {
        if (grid.IsCellOutside(item.row, item.column)) {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock() {
    current_block.Rotate();
    if (IsBlockOutside() || BlockFits() == false) {
        current_block.UndoRotation();
    }
    else {
        PlaySound(rotate_sound);
    }
}

void Game::LockBlock() {
    std::vector<Position> tiles = current_block.GetCellPositions();
    for (Position item: tiles) {
        grid.grid[item.row][item.column] = current_block.id;
    }
    current_block = next_block;
    if (BlockFits() == false) {
        game_over = true;
    }
    next_block = GetRandomBlock();
    int rows_cleared = grid.ClearFullRows();
    if (rows_cleared > 0) {
        PlaySound(clear_sound);
        UpdateScore(rows_cleared, 0);
    }
}

bool Game::BlockFits() {
    std::vector<Position> tiles = current_block.GetCellPositions();
    for (Position item: tiles) {
            if (grid.IsCellEmpty(item.row, item.column) == false) {
            return false;
        }
    }
    return true;
}

void Game::Reset() {
    grid.Initialize();
    blocks = GetAllBlocks();
    current_block = GetRandomBlock();
    next_block = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int lines_cleared, int move_down_points) {
    switch (lines_cleared) {
        case 1:
            score += 100;
            break;
        case 2:
            score += 300;
            break;
        case 3:
            score += 500;
            break;
        default:
            break;
    }

    score += move_down_points;
}