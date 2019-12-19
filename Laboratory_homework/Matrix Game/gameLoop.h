#pragma once

#include "gameMelody.h"
#include "joystick.h"
#include "globals.h"
#include "lcdMenu.h"

// GAME PARAMETERS
const int gameWidth = 8; // matrix is bordered
const int gameHeight = 8  ;

// max size of a tetromino, accounted for rotation
const int tetrominoWidth = 4;
const int tetrominoHeight = 4;

// GAME GLOBALS

// the board
byte gameMatrix[gameHeight][gameWidth];

// the tetromino/shape
byte tetromino[tetrominoHeight][tetrominoWidth];
int tetrominoX, tetrominoY; // top left coordinates on matrix
byte noOfRotations = 0;

// global timers
bool spawnNextShape = true;
unsigned long lastFall = 0;
int fallInterval = 1200;
// able to move tetromino after it falls
int gracePeriod = 400;
unsigned long lastGrace = 0;
bool graceStarted = false;

unsigned long spawnStart = 0;
int spawnDelay = 600;

// DEBUG METHODS

// debug function that spews out the matrix values to the monitor
void printMatrix() {
  Serial.println("{");
  for (byte i = 0; i < gameHeight; i++) {
    for (byte j = 0; j < gameWidth; j++) {
      Serial.print(gameMatrix[i][j]);
    }
    Serial.println("");
  }
  Serial.println("}");
}

void printTetromino() {
  Serial.println("{");
  for (byte i = 0; i < tetrominoHeight; i++) {
    for (byte j = 0; j < tetrominoWidth; j++) {
      Serial.print(tetromino[i][j]);
    }
    Serial.println("");
  }
  Serial.println("}");
}

// Tetromino methods

// clears the tetromino
void clearTetromino() {
  for (byte i = 0; i < tetrominoHeight; i++) {
    for (byte j = 0; j < tetrominoWidth; j++) {
      tetromino[i][j] = 0;
    }
  }
}

// populates the tetromino matrix with an L, at an offset
void constructL(byte offset) {
  
  clearTetromino();
  
  // how far from the top left it is
  tetrominoX = offset;
  tetrominoY = 0; // spawns it at the top

  tetromino[1][0] = 2;
  tetromino[1][1] = 2;
  tetromino[1][2] = 2;
  tetromino[0][2] = 2;
}

// same, but with a square
void constructSquare(byte offset) {
  clearTetromino();
  
  // how far from the top left it is
  tetrominoX = offset;
  tetrominoY = 0; // spawns it at the top

  tetromino[1][1] = 2;
  tetromino[1][2] = 2;
  tetromino[2][1] = 2;
  tetromino[2][2] = 2;
}

// same, but with a line
void constructLine(byte offset) {
  clearTetromino();
  
  // how far from the top left it is
  tetrominoX = offset;
  tetrominoY = 0; // spawns it at the top

  tetromino[1][0] = 2;
  tetromino[1][1] = 2;
  tetromino[1][2] = 2;
  tetromino[1][3] = 2;
}

// same, but with a T
void constructT(byte offset) {
  clearTetromino();
  
  // how far from the top left it is
  tetrominoX = offset;
  tetrominoY = 0; // spawns it at the top

  tetromino[1][0] = 2;
  tetromino[1][1] = 2;
  tetromino[0][1] = 2;
  tetromino[1][2] = 2;
}

// same, but with a Z
void constructZ(byte offset) {
  clearTetromino();
  
  // how far from the top left it is
  tetrominoX = offset;
  tetrominoY = 0; // spawns it at the top

  tetromino[1][0] = 2;
  tetromino[1][1] = 2;
  tetromino[0][1] = 2;
  tetromino[0][2] = 2;
}

bool tetrominoFits(int coordX, int coordY) {
  for (int i = coordY; i < coordY + tetrominoHeight; i++) {
    for (int j = coordX; j < coordX + tetrominoWidth; j++) {
      // if the bit is on
      if (tetromino[i - coordY][j - coordX] == 2) {
        // bounds check
        if (i >= gameHeight || i < 0 || j >= gameWidth || j < 0) {
          return false;
        }
        // collision check
        if (gameMatrix[i][j] != 0) {
          return false;
        }
//        if (i >= gameHeight || j >= gameWidth || i < 0 || j < 0 || (gameMatrix[i][j] != 0)) {
//          printTetromino();
//          Serial.println(i);
//          Serial.println(j);
//          Serial.println(coordY);
//          Serial.println(coordX);
//          return false;
//        }
      }
//      if (i >= 0 && i < gameHeight && j >= 0 && j < gameWidth) {
//          if (tetromino[i - coordY][j - coordX] == 2 && gameMatrix[i][j] != 0) {
//            return false;
//          }
//      }
    }
  }

  // no collisions, therefore it fits
  return true;
}

void tetrominoRotateRight() {
  for (byte i = 0; i < tetrominoHeight / 2; i++) {
    for (byte j = i; j < tetrominoWidth - i - 1; j++) {
      byte tmp = tetromino[i][j];

      tetromino[i][j] = tetromino[j][tetrominoWidth - 1 - i];
      tetromino[j][tetrominoWidth - 1 - i] = tetromino[tetrominoWidth - 1 - i][tetrominoWidth - 1 - j];
      tetromino[tetrominoWidth - 1 - i][tetrominoWidth - 1 - j] = tetromino[tetrominoWidth - 1 - j][i];
      tetromino[tetrominoWidth - 1 - j][i] = tmp;
    }
  }
}

void tetrominoRotateLeft() {
  tetrominoRotateRight();
  tetrominoRotateRight();
  tetrominoRotateRight();
//  // transpose
//  for (byte i = 0; i < tetrominoHeight; i++) {
//    for (byte j = i; j < tetrominoWidth; j++) {
//      swap(tetromino[i][j], tetromino[j][i]);
//    }
//  }
//  
//  // reverse rows
//  for (byte i = 0; i < tetrominoHeight; i++) {
//    for (byte j = 0, k = tetrominoHeight - 1; j < k; j++, k--) {
//      swap(tetromino[j][i], tetromino[k][i]);
//    }
//  }
}

bool tetrominoFell() {
  if (!tetrominoFits(tetrominoX, tetrominoY + 1)) {
    return true;
  }

  return false;
}

// makes tetromino static
void makeTetrominoStatic() {
  for (int i = tetrominoY; i < tetrominoY + tetrominoHeight; i++) {
    for (int j = tetrominoX; j < tetrominoX + tetrominoWidth; j++) {
      if (tetromino[i - tetrominoY][j - tetrominoX] == 2)  {
        
        gameMatrix[i][j] = 1;
      }
    }
  }
  clearTetromino();
}

// makes current tetromino static and spawns the next one
void nextTetromino() {
  // determine the next shape
  
  byte randValue = random(0, 5);

  if (randValue == 0) {
    constructL(gameWidth / 2);
  } else if (randValue == 1) {
    constructSquare(gameWidth / 2);
  } else if (randValue == 2) {
    constructLine(gameWidth / 2);
  } else if (randValue == 3) {
    constructT(gameWidth / 2);
  } else if (randValue == 4) {
    constructZ(gameWidth / 2);
  }

  tetrominoY = 0;
}

// renders the tetromino on the game matrix
void renderTetromino() {
  for (byte i = 0; i < tetrominoHeight; i++) {
    for (byte j = 0; j < tetrominoWidth; j++) {
      if (tetromino[i][j])
        lc.setLed(0, tetrominoX + j, tetrominoY + i, HIGH);
    }
  }
}

// Matrix methods

// clear matrix and set borders
void initGame() {
  for (byte i = 0; i < gameHeight; i++) {
    for (byte j = 0; j < gameWidth; j++) {
      gameMatrix[i][j] = 0;
    }
  }

  gameStartTime = currentTime;
  randomSeed(analogRead(A5) + currentTime);
}

// render method
void renderMatrix() {
  for (byte i = 0; i < gameHeight; i++) {
    for (byte j = 0; j < gameWidth; j++) {
      lc.setLed(0, j, i, gameMatrix[i][j]);
    }
  }
}

void clearLines() {
  for (int i = gameHeight - 1; i >= 0; i--) {
    bool lineFull = true;
    for (byte j = 0; j < gameWidth; j++) {
      if (gameMatrix[i][j] == 0) {
        lineFull = false;
        break;
      }
    }
    // clear the line
    if (lineFull) {
      for (byte j = 0; j < gameWidth; j++) {
        gameMatrix[i][j] = 0;
      }

      updateScore(20);
    } else {
      continue;
    }

    // move the lines up down
    for (int k = i; k >= 1; k--) {
      playSFX(2500);
      for (byte j = 0; j < gameWidth; j++) {
        gameMatrix[k][j] = gameMatrix[k-1][j];
      }
    }
  }
}

void gameLoop() {
  // INPUT //

  if (joyStickState == JOY_LEFT) {
    if (tetrominoFits(tetrominoX - 1, tetrominoY)) {
      tetrominoX--; 
    }
  }
  else if (joyStickState == JOY_RIGHT) {
    if (tetrominoFits(tetrominoX + 1, tetrominoY)) {
      tetrominoX++;
    }
  }
  else if (joyStickState == JOY_DOWN) {
    if (tetrominoFits(tetrominoX, tetrominoY + 1)) {
      playSFX(400);
      tetrominoY++;

      updateScore(1);

      // reset fall interval
      lastFall = currentTime;
    }
  } else if (joyStickState == JOY_UP) {
    tetrominoRotateRight();

    if (!tetrominoFits(tetrominoX, tetrominoY)) {
      tetrominoRotateLeft();
    } else {
      noOfRotations++;
    }

    if (noOfRotations > 3) {
      updateScore(-15);
    }
  }

  // GAME LOGIC //

  // update current time
  if (currentTime - gameStartTime >= 1000) {
    updateTimeLeft();
    gameStartTime = currentTime;
  }

  // fall
  if (currentTime > lastFall + fallInterval) {
    playSFX(400);
    lastFall = currentTime;
    if (tetrominoFits(tetrominoX, tetrominoY + 1)) {
     tetrominoY++; 
    }
  }

  // if the tromino fell signal for the next shape to spawn
  if (tetrominoFell()) {
    playSFX(1600);
    // start grace period
    if (!graceStarted) {
      graceStarted = true;
      lastGrace = currentTime;
    }
  
    // make it static after grace period
    if (currentTime > lastGrace + gracePeriod) {
      graceStarted = false;
      
      // make it static and clear the tetromino matrix
      makeTetrominoStatic();

      updateScore(10);

      // clear the full lines
      clearLines();
    
      spawnStart = currentTime;

      spawnNextShape = true;
    }
  } else if (graceStarted) {
    graceStarted = false;
  }


  // if the delay passed, spawn the next tetromino
  if (spawnNextShape && currentTime > spawnStart + spawnDelay) {
    playSFX(2000);

    nextTetromino();

    // update fall interval
    lastFall = currentTime;

    // if the spawned tetromino doesn't fit, lose the game
    if (!tetrominoFits(tetrominoX, tetrominoY)) {
      gameStarted = false;
      gameInitialized = false;
      
      menu();
      menuChanged = true;
      viewChanged = true;
      gameInfoChanged = true;
      return;
    }

    spawnNextShape = false;
  }

  // RENDER & SOUND //
  
  renderMatrix();
  renderTetromino();
  
  playMelody();  
}
