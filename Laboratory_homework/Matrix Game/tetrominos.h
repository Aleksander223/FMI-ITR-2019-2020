#pragma once

/*
 * This document defines the clear method for the tetromino, and the shapes it can have
 * 
 * Why does the tetromino matrix have padding?
 * Because I want to respect the Tetris SRS
 * See more info here: https://tetris.wiki/Super_Rotation_System
 */

// max size of a tetromino, accounted for rotation
const int tetrominoWidth = 4;
const int tetrominoHeight = 4;

// number of shapes
const int noOfShapes = 7;

// the tetromino/shape
byte tetromino[tetrominoHeight][tetrominoWidth];
int tetrominoX, tetrominoY; // top left coordinates on matrix
byte noOfRotations = 0;

int verticalOffset = 0; // this will spawn shapes from further up

// clears the tetromino
void clearTetromino()
{
    for (byte i = 0; i < tetrominoHeight; i++)
    {
        for (byte j = 0; j < tetrominoWidth; j++)
        {
            tetromino[i][j] = 0;
        }
    }

    // reset values
    noOfRotations = 0;  
}

// rotates tetromino right
// just a simple matrix rotation
void tetrominoRotateRight()
{
    for (byte i = 0; i < tetrominoHeight / 2; i++)
    {
        for (byte j = i; j < tetrominoWidth - i - 1; j++)
        {
            byte tmp = tetromino[i][j];

            tetromino[i][j] = tetromino[j][tetrominoWidth - 1 - i];
            tetromino[j][tetrominoWidth - 1 - i] = tetromino[tetrominoWidth - 1 - i][tetrominoWidth - 1 - j];
            tetromino[tetrominoWidth - 1 - i][tetrominoWidth - 1 - j] = tetromino[tetrominoWidth - 1 - j][i];
            tetromino[tetrominoWidth - 1 - j][i] = tmp;
        }
    }
}

void tetrominoRotateLeft()
{
    tetrominoRotateRight();
    tetrominoRotateRight();
    tetrominoRotateRight();
}

// populates the tetromino matrix with an L, at an offset
void constructL(byte offset)
{

    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[1][0] = 2;
    tetromino[1][1] = 2;
    tetromino[1][2] = 2;
    tetromino[0][2] = 2;
}

// same, but with a square
void constructSquare(byte offset)
{
    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[1][1] = 2;
    tetromino[1][2] = 2;
    tetromino[2][1] = 2;
    tetromino[2][2] = 2;
}

// same, but with a line
void constructLine(byte offset)
{
    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[1][0] = 2;
    tetromino[1][1] = 2;
    tetromino[1][2] = 2;
    tetromino[1][3] = 2;
}

// same, but with a T
void constructT(byte offset)
{
    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[1][0] = 2;
    tetromino[1][1] = 2;
    tetromino[0][1] = 2;
    tetromino[1][2] = 2;
}

// same, but with a S
void constructS(byte offset)
{
    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[1][0] = 2;
    tetromino[1][1] = 2;
    tetromino[0][1] = 2;
    tetromino[0][2] = 2;
}

// same, but with a J
void constructJ(byte offset)
{

    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[1][0] = 2;
    tetromino[1][1] = 2;
    tetromino[1][2] = 2;
    tetromino[2][2] = 2;
}

// same, but with a Z
void constructZ(byte offset)
{
    clearTetromino();

    // how far from the top left it is
    tetrominoX = offset;
    tetrominoY = verticalOffset; // spawns it at the top

    tetromino[0][0] = 2;
    tetromino[0][1] = 2;
    tetromino[1][1] = 2;
    tetromino[1][2] = 2;
}
