#pragma once
#include <vector>
#include <map>
#include "position.h"
#include "colors.h"

class Block {
public:
    Block();
    void Draw(int offset_x, int offset_y);
    void Move(int rows, int columns);
    std::vector<Position> GetCellPositions();
    void Rotate();
    void UndoRotation();
    int id;
    std::map<int, std::vector<Position>> cells;
private:
    int cell_size;
    int rotation_state;
    std::vector<Color> colors;
    int row_offset;
    int column_offset;
};