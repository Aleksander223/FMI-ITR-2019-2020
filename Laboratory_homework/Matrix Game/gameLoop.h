#pragma once

#include "gameMelody.h"
#include "joystick.h"
#include "globals.h"
#include "lcdMenu.h"
#include "tetrominos.h"

/*
 * How all of this works
 * We have a matrix which displays the already placed pieces
 * We also have a tetromino matrix which is used to display the currently falling tetromino
 * When we render stuff, the matrix is displayed first, then the tetromino
 * 
 * 
 * Why this approach, and not a single matrix?
 * 1. It's easier to rotate the tetromino, since it's state is independent of the matrix, we just have to
 * do a matrix collision check
 * 2. It's easier to move it as well, we just update it's coordinates if it passes the collision test
 * 3. It's easier to define multiple shapes, by just modifying values from that matrix
 * 4. Instead of updating the whole matrix when the tetromino changes, we just have to do local collision
 * and change just what's inside the tetromino matrix (way more efficient than iterating over the whole matrix)
*/

// GAME PARAMETERS
const int gameWidth = 8; // matrix is bordered
const int gameHeight = 8;

// GAME GLOBALS

// the board
byte gameMatrix[gameHeight][gameWidth];

// game timers
bool spawnNextShape = true;
unsigned long lastFall = 0;
int fallInterval = 1200;
// this is used for game time limit logic and for the lcd
// basically it's useful for displaying when a second passed
unsigned long lastTimerStart = 0;

// able to move tetromino after it falls
int gracePeriod = 400;
unsigned long lastGrace = 0;
bool graceStarted = false;

unsigned long spawnStart = 0;
int spawnDelay = 600;

// DEBUG METHODS

// debug function that spews out the matrix values to the monitor
void printMatrix()
{
  Serial.println("{");
  for (byte i = 0; i < gameHeight; i++)
  {
    for (byte j = 0; j < gameWidth; j++)
    {
      Serial.print(gameMatrix[i][j]);
    }
    Serial.println("");
  }
  Serial.println("}");
}

void printTetromino()
{
  Serial.println("{");
  for (byte i = 0; i < tetrominoHeight; i++)
  {
    for (byte j = 0; j < tetrominoWidth; j++)
    {
      Serial.print(tetromino[i][j]);
    }
    Serial.println("");
  }
  Serial.println("}");
}

bool tetrominoFits(int coordX, int coordY)
{
  for (int i = coordY; i < coordY + tetrominoHeight; i++)
  {
    for (int j = coordX; j < coordX + tetrominoWidth; j++)
    {
      // if the bit is on
      if (tetromino[i - coordY][j - coordX] == 2)
      {
        // bounds check
        if (i >= gameHeight || i < 0 || j >= gameWidth || j < 0)
        {
          return false;
        }
        // collision check
        if (gameMatrix[i][j] != 0)
        {
          return false;
        }
      }
    }
  }

  // no collisions, therefore it fits
  return true;
}

// returns whether the tetromino collided with a static piece
bool tetrominoFell()
{
  if (!tetrominoFits(tetrominoX, tetrominoY + 1))
  {
    return true;
  }

  return false;
}

// makes tetromino static
void makeTetrominoStatic()
{
  for (int i = tetrominoY; i < tetrominoY + tetrominoHeight; i++)
  {
    for (int j = tetrominoX; j < tetrominoX + tetrominoWidth; j++)
    {
      if (tetromino[i - tetrominoY][j - tetrominoX] == 2)
      {

        gameMatrix[i][j] = 1;
      }
    }
  }
  clearTetromino();
}

// makes current tetromino static and spawns the next one
void nextTetromino()
{
  // determine the next shape

  byte randValue = random(0, 5);

  if (randValue == 0)
  {
    constructL(gameWidth / 2);
  }
  else if (randValue == 1)
  {
    constructSquare(gameWidth / 2);
  }
  else if (randValue == 2)
  {
    constructLine(gameWidth / 2);
  }
  else if (randValue == 3)
  {
    constructT(gameWidth / 2);
  }
  else if (randValue == 4)
  {
    constructZ(gameWidth / 2);
  }

  tetrominoY = 0;
}

// Matrix methods

// clear matrix and set borders
void initGame()
{
  for (byte i = 0; i < gameHeight; i++)
  {
    for (byte j = 0; j < gameWidth; j++)
    {
      gameMatrix[i][j] = 0;
    }
  }

  gameStartTime = currentTime;
  randomSeed(analogRead(A5) + currentTime);
}

// clears the full lines of the matrix, from bottom to top
void clearLines()
{
  for (int i = gameHeight - 1; i >= 0; i--)
  {
    bool lineFull = true;
    for (byte j = 0; j < gameWidth; j++)
    {
      if (gameMatrix[i][j] == 0)
      {
        lineFull = false;
        break;
      }
    }
    // clear the line
    if (lineFull)
    {
      for (byte j = 0; j < gameWidth; j++)
      {
        gameMatrix[i][j] = 0;
      }

      updateScore(20);
    }
    else
    {
      continue;
    }

    // move the lines up down
    for (int k = i; k >= 1; k--)
    {
      playSFX(2500);
      for (byte j = 0; j < gameWidth; j++)
      {
        gameMatrix[k][j] = gameMatrix[k - 1][j];
      }
    }

    // decrement i so we don't skip lines
    i--;
  }
}

// render methods
void renderMatrix()
{
  for (byte i = 0; i < gameHeight; i++)
  {
    for (byte j = 0; j < gameWidth; j++)
    {
      lc.setLed(0, j, i, gameMatrix[i][j]);
    }
  }
}

// renders the tetromino on the game matrix
void renderTetromino()
{
  for (byte i = 0; i < tetrominoHeight; i++)
  {
    for (byte j = 0; j < tetrominoWidth; j++)
    {
      if (tetromino[i][j])
        lc.setLed(0, tetrominoX + j, tetrominoY + i, HIGH);
    }
  }
}

void gameLoop()
{
  // INPUT //

  // move piece left
  if (joyStickState == JOY_LEFT)
  {
    if (tetrominoFits(tetrominoX - 1, tetrominoY))
    {
      tetrominoX--;
    }
  }
  // move piece right
  else if (joyStickState == JOY_RIGHT)
  {
    if (tetrominoFits(tetrominoX + 1, tetrominoY))
    {
      tetrominoX++;
    }
  }
  // rush piece down, 1 block at a time (for better control)
  else if (joyStickState == JOY_DOWN)
  {
    if (tetrominoFits(tetrominoX, tetrominoY + 1))
    {
      playSFX(400);
      tetrominoY++;

      updateScore(1);

      // reset fall interval
      lastFall = currentTime;
    }
  }
  // rotate piece to the right
  else if (joyStickState == JOY_UP)
  {
    tetrominoRotateRight();

    if (!tetrominoFits(tetrominoX, tetrominoY))
    {
      tetrominoRotateLeft();
    }
    else
    {
      noOfRotations++;
    }

    if (noOfRotations > 3)
    {
      // if we rotate piece more than 3 times (equivalent to a left rotation)
      // then we put a penalty to the player's score
      updateScore(-15);
    }
  }

  // GAME LOGIC //

  // update level timer
  if (currentTime - lastTimerStart >= 1000)
  {
    updateTimeLeft();
    lastTimerStart = currentTime;
  }

  // fall
  if (currentTime > lastFall + fallInterval)
  {
    playSFX(400);
    lastFall = currentTime;
    if (tetrominoFits(tetrominoX, tetrominoY + 1))
    {
      tetrominoY++;
    }
  }

  // if the tromino fell signal for the next shape to spawn
  if (tetrominoFell())
  {
    playSFX(1600);
    // start grace period
    if (!graceStarted)
    {
      graceStarted = true;
      lastGrace = currentTime;
    }

    // make it static after grace period
    if (currentTime > lastGrace + gracePeriod)
    {
      graceStarted = false;

      // make it static and clear the tetromino matrix
      makeTetrominoStatic();

      updateScore(10);

      // clear the full lines
      clearLines();

      spawnStart = currentTime;

      spawnNextShape = true;
    }
  }
  else if (graceStarted)
  {
    graceStarted = false;
  }

  // if the delay passed, spawn the next tetromino
  if (spawnNextShape && currentTime > spawnStart + spawnDelay)
  {
    playSFX(2000);

    nextTetromino();

    // update fall interval
    lastFall = currentTime;

    // if the spawned tetromino doesn't fit, lose the game
    if (!tetrominoFits(tetrominoX, tetrominoY))
    {
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
