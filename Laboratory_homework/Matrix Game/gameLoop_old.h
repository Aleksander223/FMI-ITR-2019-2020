#pragma once

#include "gameMelody.h"
#include "joystick.h"
#include "globals.h"
#include "lcdMenu.h"

/*
   GAME GLOBALS
*/

byte gameMatrix[8][8];
bool spawnNextShape = true;
unsigned long lastFall = 0;
int fallInterval = 600;

/*
   TETROMINO SPAWN METHODS
*/

bool spawnSquare() {
  if (gameMatrix[0][0] == 1 || gameMatrix[0][1] == 1 || gameMatrix[1][0] == 1 || gameMatrix[1][1] == 1) {
    return false;
  }

  gameMatrix[0][0] = 2;
  gameMatrix[0][1] = 2;
  gameMatrix[1][0] = 2;
  gameMatrix[1][1] = 2;

  return true;
}

bool spawnL() {
  if (gameMatrix[0][0] == 1 || gameMatrix[1][0] == 1 || gameMatrix[2][0] == 1 || gameMatrix[2][1] == 1) {
    return false;
  }

  gameMatrix[0][0] = 2;
  gameMatrix[1][0] = 2;
  gameMatrix[2][0] = 2;
  gameMatrix[2][1] = 2;

  return true;
}


bool spawnZ() {
  if (gameMatrix[0][0] == 1 || gameMatrix[0][1] == 1 || gameMatrix[1][1] == 1 || gameMatrix[1][2] == 1) {
    return false;
  }

  gameMatrix[0][0] = 2;
  gameMatrix[0][1] = 2;
  gameMatrix[1][1] = 2;
  gameMatrix[1][2] = 2;

  return true;
}

bool spawnLine() {
  if (gameMatrix[0][0] == 1 || gameMatrix[1][0] == 1 || gameMatrix[2][0] == 1) {
    return false;
  }

  gameMatrix[0][0] = 2;
  gameMatrix[1][0] = 2;
  gameMatrix[2][0] = 2;

  return true;
}

bool spawnShape() {
  int value = random(0, 4);

  if (value == 0) {
    return spawnLine();
  } else if (value == 1) {
    return spawnL();
  } else if (value == 2) {
    return spawnZ();
  } else if (value == 3) {
    return spawnSquare();
  }
}

/*
   MATRIX DEBUG & PRINT METHODS
*/

void initGame() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      gameMatrix[i][j] = 0;
    }
  }
}

void renderMatrix() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, j, i, gameMatrix[i][j]);
    }
  }
}

// debug function that spews out the matrix values to the monitor
void printMatrix() {
  Serial.println("{");
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Serial.print(gameMatrix[i][j]);
    }
    Serial.println("");
  }
  Serial.println("}");
}

bool canFall() {
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      if ((gameMatrix[i][j] == 2 && gameMatrix[i + 1][j] == 1) || (gameMatrix[i][j] == 2 && i + 1 == 8)) {
        return false;
      }
    }
  }

  return true;
}

void updateMatrix() {
  // if the shape can fall
  if (canFall() && !spawnNextShape) {
    for (int i = 7; i >= 1; i--) {
      for (int j = 0; j < 8; j++) {
        if (gameMatrix[i - 1][j] == 2) {
          gameMatrix[i][j] = gameMatrix[i - 1][j];
          gameMatrix[i - 1][j] = 0;

          // check if it fell on boundary and play SFX
          if (!canFall()) {
            playSFX(1200);

//            // make it static
//            for (int k = 0; k < 8; k++) {
//              for (int l = 0; l < 8; l++) {
//                if (gameMatrix[i][j] == 2) {
//                  gameMatrix[i][j] = 1;
//                }
//              }
//            }

            // spawn next shape
//            spawnNextShape = true;
          }
        }
      }
    }
  }
  // else we make it static and signal for the next shape to fall
  else {
    for (int i = 7; i >= 1; i--) {
      for (int j = 0; j < 8; j++) {
        if (gameMatrix[i][j] == 2) {
          gameMatrix[i][j] = 1;
        }
      }
    }

    spawnNextShape = true;

  }
}

void updateMatrixAfterLineDeletion() {
  for (int i = 7; i >= 1; i--) {
    for (int j = 0; j < 8; j++) {
      if (gameMatrix[i - 1][j] == 1) {
        gameMatrix[i][j] = 1;
        gameMatrix[i - 1][j] = 0;
      }
    }
  }
}

/*
   TETROMINO METHODS: rotate, move
*/

// shifts tetromino left, WITHOUT CHECKING FOR COLLISION
void shiftLeft() {
  for (int j = 0; j < 7; j++) {
    for (int i = 0; i < 8; i++) {
      if (gameMatrix[i][j + 1] == 2 && gameMatrix[i][j] == 0) {
        gameMatrix[i][j] = gameMatrix[i][j + 1];
        gameMatrix[i][j + 1] = 0;
      }
    }
  }
}

// shifts tetromino right, WITHOUT CHECKING FOR COLLISION
void shiftRight() {
  for (int j = 7; j >= 1; j--) {
    for (int i = 0; i < 8; i++) {
      if (gameMatrix[i][j - 1] == 2 && gameMatrix[i][j] == 0) {
        gameMatrix[i][j] = gameMatrix[i][j - 1];
        gameMatrix[i][j - 1] = 0;
      }
    }
  }
}

// checks if it can move the shape
// moves the shape either left or right
// -1 for left, 1 for right
void moveShape(int dir) {
  // LEFT
  if (dir == -1) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        // bound check

        if (gameMatrix[i][j] == 2 && (j + dir < 0 || gameMatrix[i][j + dir] == 1)) {
          return;
        }
      }
    }
    shiftLeft();
  }
  // RIGHT
  else if (dir == 1) {
    for (int i = 0; i < 8; i++) {
      for (int j = 7 ; j >= 0; j--) {
        // bound check
        if (gameMatrix[i][j] == 2 && (j + dir > 7 || gameMatrix[i][j + dir] == 1)) {
          return;
        }
      }
    }
    shiftRight();
  }
}

// checks if last line is completed
void deleteLines() {
  bool deletedAtLeastOne = false;
  for (int i = 0; i < 8; i++) {
    bool deleteLine = true;
    for (int j = 0; j < 8; j++) {
      if (gameMatrix[i][j] != 1) {
        deleteLine = false;
        break;
      }

      if (deleteLine) {
        for (int j = 0; j < 8; j++) {
          deletedAtLeastOne = true;
          gameMatrix[i][j] = 0;
        }
      }
    }
  }

  if (deletedAtLeastOne) {
    updateMatrixAfterLineDeletion();
    playSFX(600);
  }
  
//  for (int i = 0; i < 8; i++) {
//    if (gameMatrix[7][i] == 0 || gameMatrix[7][i] == 2) {
//      return;
//    }
//  }
//
//  printMatrix();
//  for (int i = 0; i < 8; i++) {
//    gameMatrix[7][i] = 0;
//  }
//
//  updateMatrixAfterLineDeletion();
//  playSFX(600);
}

void gameLoop() {

  // spawn the next tetromino
  if (spawnNextShape) {
    // try to spawn the next shape, if we can't then we lost the game    
    if (!spawnShape()) {
      gameStarted = false;
      viewChanged = true;

      spawnNextShape = true;
      return;
    }

    spawnNextShape = false;
  }

  if (joyStickState == JOY_LEFT) {
    moveShape(-1);
  }
  else if (joyStickState == JOY_RIGHT) {
    moveShape(1);
  }

  renderMatrix();
  playMelody();

  if (currentTime > lastFall + fallInterval) {
    //    printMatrix();
    lastFall = currentTime;
    updateMatrix();
  }

  deleteLines();
}
