//
// Created by nmich on 5/12/2024.
//
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include "Block.h"
#ifndef TETRIS_DISPLAY_H
#define TETRIS_DISPLAY_H
#define matrixVec std::vector<std::vector<Block>>

std::uniform_int_distribution<int> blockTypeGen(0,5);
std::vector<Block> types{Block::L, Block::REVERSE_L, Block::S, Block::Z, Block::T, Block::LINE};
int numBackLoaded = 3;
int xWidth = 10, yHeight = 24;

class Display{
public:
    matrixVec matrix;
    DisplayBlock currentBlock;//ordered by most recent -> oldest
    std::vector<Block> nextBlocks;

    Display(){
        matrix = matrixVec(yHeight, std::vector<Block>(xWidth, Block::NONE));
    }

    void addNewBlock(){
        nextBlocks.insert(nextBlocks.begin(), types[blockTypeGen(generator)]);
        currentBlock = DisplayBlock(nextBlocks[numBackLoaded], matrix);
        nextBlocks.pop_back();
    }
    
    void cycle(){
        currentBlock.moveDown(matrix);
    }

    std::vector<int> checkForLines(){
        std::vector<int> res;
        int currLines = 0;
        for (int currLine = yHeight - 1; currLine >= 0; currLine--){
            int full = true;
            for (auto currBlock : matrix[currLine]){
                if (currBlock == Block::NONE) full = false;
            }
            if (full){
                currLines++;
                moveDownAbove(currLine);
                currLine++;
            }
            else{
                if (currLines != 0){
                    res.push_back(currLines);
                    currLines = 0;
                }
            }
        }
        if (currLines != 0){
            res.push_back(currLines);
        }
        return res;
    }

    void moveDownAbove(int line){
        for (int currLine = line; currLine >= 0; currLine++){
            for (int i = 0; i < xWidth; i++){
                if (currLine == 0) matrix[line][i] = Block::NONE;
                else matrix[line][i] = matrix[line - 1][i];
            }
        }
    }
};

#endif //TETRIS_DISPLAY_H
