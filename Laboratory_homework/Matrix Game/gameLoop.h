#pragma once

#include "gameMelody.h"
#include "joystick.h"
#include "globals.h"
#include "lcdMenu.h"

/*
 * GAME GLOBALS
 */

byte gameMatrix[8][8];
bool spawnNextShape = true;
unsigned long lastFall = 0;
int fallInterval = 600;

/*
 * TETROMINO SPAWN METHODS
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

/*
 * MATRIX DEBUG & PRINT METHODS
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

void updateMatrix() {
  // check collisions
  bool canFall = true;
  for (int i = 7; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      if ((gameMatrix[i][j] == 2 && gameMatrix[i+1][j] == 1) || (gameMatrix[i][j] == 2 && i + 1 == 8)) {
        canFall = false;
        break;
      }
    }
  }

  // if the shape can fall
  if (canFall) {
    for (int i = 7; i >=1; i--) {
      for (int j = 0; j < 8; j++) {
        if (gameMatrix[i-1][j] == 2) {
          gameMatrix[i][j] = gameMatrix[i-1][j];
          gameMatrix[i-1][j] = 0;

          // check if it fell on boundary and play SFX
          if (gameMatrix[i + 1][j] == 1 || i + 1 == 8) {
            playSFX();
          }
        }
      }
    }
  }
  // else we make it static and signal for the next shape to fall 
  else {
    for (int i = 7; i >=1; i--) {
      for (int j = 0; j < 8; j++) {
        if (gameMatrix[i][j] == 2) {
          gameMatrix[i][j] = 1;
        }
      }
    }

    spawnNextShape = true;
    
  }
}

/*
 * TETROMINO METHODS: rotate, move
 */

// checks if it can move the shape
// moves the shape either left or right
// -1 for left, 1 for right
void moveShape(int dir) {
  // LEFT
  if (dir == -1) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        // bound check
        if (gameMatrix[i][j] == 2 && j + dir >= 0 && gameMatrix[i][j + dir] == 0) {
           // move shape left
           
        } else if (gameMatrix[i][j] == 2){
          return;
        }
      }
    }
  }
  // RIGHT
  else if (dir == 1) {
    for (int i = 0; i <8; i++) {
      for (int j = 7 ; j >= 0; j--) {
        // bound check 
        if (gameMatrix[i][j] == 2 && j + dir <= 7 && gameMatrix[i][j + dir] == 0) {
           // move shape right
        } else if (gameMatrix[i][j] == 2){
          return;
        }
      }
    }
  }
}

void gameLoop() {

  // spawn the next tetromino
  if (spawnNextShape) {
    // try to spawn the next shape, if we can't then we lost the game
    if(!spawnL()) {
      gameStarted = false;
      viewChanged = true;
    }
    
    spawnNextShape = false;
  }

  if (joyStickState == JOY_LEFT) {
    Serial.println("MOVE SHAPE LEFT");
    moveShape(-1);
  }
  else if (joyStickState == JOY_RIGHT) {
    Serial.println("MOVE SHAPE RIGHT");
    moveShape(1);
  }
  
  renderMatrix();
  playMelody();

  if (currentTime > lastFall + fallInterval) {
    printMatrix();
    lastFall = currentTime;
    updateMatrix();
  }
}
