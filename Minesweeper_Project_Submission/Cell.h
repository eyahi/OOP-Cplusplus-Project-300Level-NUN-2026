#ifndef CELL_H
#define CELL_H
//Represents the properties used to define each cell 
struct Cell {
    bool isMine{false}; //A cell that doesn't contain a mine
    bool isRevealed{false}; //A cell that hasn't been revealed
    bool isFlagged{false}; //A cell that hasn't been flagged
    int neighborCount{0}; //The number of mines around a cell, the default being zero
};

#endif