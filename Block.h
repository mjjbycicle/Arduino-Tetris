//
// Created by nmich on 5/12/2024.
//
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#ifndef TETRIS_BLOCK_H
#define TETRIS_BLOCK_H

#define matrixVec std::vector<std::vector<Block>>
enum class Block {
    L = 93,
    REVERSE_L = 214,
    S = 226,
    Z = 27,
    LINE = 84,
    T = 197,
    NONE = 0
};

std::default_random_engine generator;
std::uniform_int_distribution<int> rotationGen(0,3);


matrixVec getVec(Block b) {
    switch (b) {
        case Block::L:
            return matrixVec
                    {
                            {Block::NONE, Block::L, Block::NONE},
                            {Block::NONE, Block::L, Block::NONE},
                            {Block::NONE, Block::L, Block::L}
                    };
        case Block::REVERSE_L:
            return matrixVec
                    {
                            {Block::NONE,      Block::REVERSE_L, Block::NONE},
                            {Block::NONE,      Block::REVERSE_L, Block::NONE},
                            {Block::REVERSE_L, Block::REVERSE_L, Block::NONE}
                    };
        case Block::S:
            return matrixVec
                    {
                            {Block::NONE, Block::NONE, Block::NONE},
                            {Block::NONE, Block::S,    Block::S},
                            {Block::S,    Block::S,    Block::L}
                    };
        case Block::Z:
            return matrixVec
                    {
                            {Block::NONE, Block::NONE, Block::NONE},
                            {Block::Z,    Block::Z,    Block::NONE},
                            {Block::NONE, Block::Z,    Block::Z}
                    };
        case Block::T:
            return matrixVec
                    {
                            {Block::NONE, Block::NONE, Block::NONE},
                            {Block::NONE, Block::T,    Block::NONE},
                            {Block::T,    Block::T,    Block::T}
                    };
        case Block::LINE:
            return matrixVec
                    {
                            {Block::NONE, Block::LINE, Block::NONE, Block::NONE},
                            {Block::NONE, Block::LINE, Block::NONE, Block::NONE},
                            {Block::NONE, Block::LINE, Block::NONE, Block::NONE},
                            {Block::NONE, Block::LINE, Block::NONE, Block::NONE},
                    };
    }
}

bool occupied(matrixVec display, int r, int c);

void erase();

void display_matrix(matrixVec board);

class DisplayBlock {
public:
    Block blockType;
    int num_rot;
    int x;
    int y;
    matrixVec displayVec;
    bool set = false;

    DisplayBlock(Block b, int num_rot, int x, int y, matrixVec& matrix) : blockType(b) {
        this->num_rot = num_rot;
        this->x = x;
        this->y = y;
        this->displayVec = getVec(b);
        rotate(num_rot);
        display(matrix, *this);
    }

    DisplayBlock(Block b, matrixVec& matrix){
        this->blockType = b;
        this->num_rot = rotationGen(generator);
        this->x = 5;
        this->y = 23;
        this->displayVec = getVec(blockType);
        rotate(num_rot);
        display(matrix, *this);
    }

    //clockwise
    void rotate() {
        matrixVec newVec(displayVec.size(), std::vector<Block>(displayVec.size(), Block::NONE));
        for (int i = 0; i < newVec.size(); i++) {
            for (int j = 0; j < newVec.size(); j++) {
                newVec[j][newVec.size() - i - 1] = displayVec[i][j];
            }
        }
        displayVec = newVec;
    }

    void rotate(int times) {
        for (int i = 0; i < times; i++) {
            rotate();
        }
    }

    void drop(matrixVec& matrix){
        remove_display(matrix, *this);
        for (int i = 0; i < 24; i++){
            moveDown(matrix);
        }
        display(matrix, *this);
    }

    void move(matrixVec& matrix, bool movingLeft) {
        remove_display(matrix, *this);
        if (canMoveSides(matrix, movingLeft)) {
            x -= movingLeft ? 1 : -1;
        }
        display(matrix, *this);
    }

    void moveDown(matrixVec& matrix) {
        remove_display(matrix, *this);
        if (canMoveDown(matrix)) {
            y--;
        }
        else {
            set = true;
        }
        display(matrix, *this);
    }

    bool canMoveDown(matrixVec matrix) {
        int rowstart = matrix.size() - y - 1;
        int colstart = x;
        if (blockType == Block::LINE) {
            rowstart -= 2;
            colstart -= 2;
        } else {
            rowstart--;
            colstart--;
        }
        for (int i = 1; i <= (int)displayVec.size(); i++) {
            for (int j = 0; j < (int)displayVec.size(); j++) {
                if (occupied(displayVec, i - 1, j)
                    and occupied(matrix, rowstart + i, colstart + j)) {
                    if (i != displayVec.size()){
                        if (displayVec[i][j] == Block::NONE){
                            return false;
                        }
                    }
                    else
                        return false;
                }
            }
        }
        return true;
    }

    bool canMoveSides(matrixVec matrix, bool movingLeft) {
        int rowstart = matrix.size() - y - 1;
        int colstart = x;
        int modifier = movingLeft ? -1 : 1;
        if (blockType == Block::LINE) {
            rowstart -= 2;
            colstart -= 2;
        } else {
            rowstart--;
            colstart--;
        }
        for (int i = 0; i < (int)displayVec.size(); i++) {
            for (int j = (int)modifier;displayVec.size() + modifier; j++) {
                if (occupied(displayVec, i, j - modifier)
                    and occupied(matrix, rowstart + i, colstart + j)) {
                    if (j != -1 && j != displayVec.size()){
                        if (displayVec[i][j] == Block::NONE)
                            return false;
                    }
                    else
                        return false;
                }
            }
        }
        return true;
    }

    static void display(matrixVec &display, DisplayBlock block) {
        int rowstart, colstart;
        if (block.blockType == Block::LINE) {
            rowstart = display.size() - block.y - 3;
            colstart = block.x - 2;
        } else {
            rowstart = display.size() - block.y - 2;
            colstart = block.x - 1;
        }
        for (int i = 0; i < block.displayVec.size(); i++) {
            for (int j = 0; j < block.displayVec.size(); j++) {
                if (block.displayVec[i][j] == Block::NONE) continue;
                else {
                    display[i + rowstart][j + colstart] = block.displayVec[i][j];
                }
            }
        }
    }

    static void remove_display(matrixVec &display, DisplayBlock block) {
        int rowstart, colstart;
        if (block.blockType == Block::LINE) {
            rowstart = display.size() - block.y - 3;
            colstart = block.x - 2;
        } else {
            rowstart = display.size() - block.y - 2;
            colstart = block.x - 1;
        }
        for (int i = 0; i < block.displayVec.size(); i++) {
            for (int j = 0; j < block.displayVec.size(); j++) {
                if (block.displayVec[i][j] == Block::NONE) continue;
                else {
                    display[i + rowstart][j + colstart] = Block::NONE;
                }
            }
        }
    }
};


bool occupied(matrixVec display, int r, int c) {
    if (r == display.size()) return true;
    if (c == -1) return true;
    if (c == display[0].size()) return true;
    if (display[r][c] != Block::NONE) return true;
    return false;
}

std::string getColor(Block b) {
    return std::string{} + "\033[48;5;" + std::to_string((int) b) + "m\033[38;5;" + std::to_string((int) b) +
           "m__\033[0m";
}

void display_matrix(matrixVec board) {
    std::cout << "\033[H";
    for (auto i: board) {
        for (auto j: i) {
            std::cout << getColor(j);
        }
        std::cout << std::endl;
    }
}

void erase() {
    std::cout << "\033[H";
    std::cout << "\033[0J";
}


#endif //TETRIS_BLOCK_H
