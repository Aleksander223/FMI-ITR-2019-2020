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
int fallInterval = startingFallInterval;
// this is used for game time limit logic and for the lcd
// basically it's useful for displaying when a second passed
unsigned long lastTimerStart = 0;

// able to move tetromino after it falls
int gracePeriod = startingGracePeriod;
unsigned long lastGrace = 0;
bool graceStarted = false;

unsigned long spawnStart = 0;
int spawnDelay = startingSpawnDelay;

// Game info methods
void updateScore(int value)
{
  if (value < 0 && level * -value >= gameScore)
  {
    gameScore = 0;
  }
  else
  {
    gameScore += level * value;
  }

  gameInfoChanged = true;
}

void resetGoal()
{
  currentGoal = 0;

  gameInfoChanged = true;
}

void updateLevel()
{
  level++;
  goal += 1;

  // reset goal progress
  resetGoal();

  if (timeLimit - 5 > 15)
  {
    timeLimit -= 5;
  }

  if (spawnDelay - spawnDelayDecrement > spawnDelayMin)
  {
    spawnDelay -= spawnDelayDecrement;
  }

  if (fallInterval - fallIntervalDecrement > fallIntervalMin)
  {
    fallInterval -= fallIntervalDecrement;
  }

  gameInfoChanged = true;
}

void updateGoal()
{
  currentGoal++;

  gameInfoChanged = true;
}

void updateTimeLeft()
{
  timeLeft--;

  gameInfoChanged = true;
}

void resetTimeLeft()
{
  timeLeft = timeLimit;
}

// resets all params to their default value
void resetGame()
{
  resetGoal();
  goal = startingGoal;
  timeLimit = startingTimeLimit;
  resetTimeLeft();

  level = startingLevel;

  fallInterval = startingFallInterval;
  gracePeriod = startingGracePeriod;
  spawnDelay = startingSpawnDelay;

  gameScore = 0;
}

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
        // if it's too high up, we'll just return true
        if (i < 0 && j >= 0 && j < gameWidth)
        {
          return true;
        }
        // bounds check
        if (i >= gameHeight || j >= gameWidth || j < 0)
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

  byte randValue = random(0, noOfShapes);

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
  else if (randValue == 5)
  {
    constructJ(gameWidth / 2);
  }
  else if (randValue == 6)
  {
    constructS(gameWidth / 2);
  }
}

// Matrix methods

// clear matrix
void clearMatrix()
{
  for (byte i = 0; i < gameHeight; i++)
  {
    for (byte j = 0; j < gameWidth; j++)
    {
      gameMatrix[i][j] = 0;
    }
  }
}

// clear matrix and set borders
void initGame()
{
  resetGame();
  clearMatrix();

  lastTimerStart = currentTime;
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

    // update goal
    updateGoal();
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

// loses the game
void loseGame()
{
  gameStarted = false;
  gameInitialized = false;

  menuChanged = true;

  currentView = 4;

  menu();

  // write score
  STRUCT_SCORE highestScore = readHighScore();
  if (highestScore.score < gameScore)
  {
    writeHighScore(playerName, gameScore);
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
  // reset shape
  else if (joyStickState == JOY_SW)
  {
    playSFX(2000);

    nextTetromino();

    // update fall interval
    lastFall = currentTime;

    // if the spawned tetromino doesn't fit, lose the game
    if (!tetrominoFits(tetrominoX, tetrominoY))
    {
      loseGame();
    }

    spawnNextShape = false;

    // apply a penalty
    updateScore(-20);
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

  // LEVEL ADVANCE
  if (currentGoal >= goal)
  {
    updateLevel();

    // reset time
    resetTimeLeft();

    // reset matrix as well
    clearMatrix();
  }

  // TIME LIMIT FAIL
  if (timeLeft <= 0)
  {
    loseGame();
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
      loseGame();
    }

    spawnNextShape = false;
  }

  // RENDER & SOUND //

  renderMatrix();
  renderTetromino();

  playMelody();
}
