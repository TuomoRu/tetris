#include "block.h"

Block::Block() {
    cell_size = 30;
    rotation_state = 0;
    colors = GetCellColors();
    row_offset = 0;
    column_offset = 0;
}

void Block::Draw(int offset_x, int offset_y) {
    std::vector<Position> tiles = GetCellPositions();
    for (Position item: tiles) {
        DrawRectangle(item.column * cell_size + offset_x, item.row * cell_size + offset_y, cell_size - 1, cell_size - 1, colors[id]);
    }
}

void Block::Move(int rows, int columns) {
    row_offset += rows;
    column_offset += columns;
}

std::vector<Position> Block::GetCellPositions() {
    std::vector<Position> tiles = cells[rotation_state];
    std::vector<Position> moved_tiles;
    for (Position item: tiles) {
        Position new_pos = Position(item.row + row_offset, item.column + column_offset);
        moved_tiles.push_back(new_pos);
    }
    return moved_tiles;
}

void Block::Rotate() {
    rotation_state++;
    if (rotation_state == (int)cells.size()) {
        rotation_state = 0;
    }
}

void Block::UndoRotation() {
    rotation_state--;
    if (rotation_state == -1) {
        rotation_state = cells.size() - 1;
    }
}