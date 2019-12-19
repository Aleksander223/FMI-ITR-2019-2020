#pragma once

#include "globals.h"
#include "joystick.h"
#include "gameScore.h"

byte cursorPosition = 0;
const byte numStrings = 4;
byte currentView = 0;

char menuStrings[numStrings][16] = {
    "Start",
    "Settings",
    "Highscore",
    "Info"};

char infoMenuStrings[numStrings][16] = {
    "Endless Tetris",
    "bit.ly/2YV4mg4",
    "Adam Alexandru V",
    "@unibucrobotics"};

void updateCursor()
{
  if (joyStickState == JOY_DOWN)
  {
    cursorPosition = (cursorPosition + 1) % numStrings;
    menuChanged = true;
  }
  else if (joyStickState == JOY_UP)
  {
    if (cursorPosition == 0)
    {
      cursorPosition = numStrings - 1;
    }
    else
    {
      cursorPosition--;
    }
    menuChanged = true;
  }
  else if (joyStickState == JOY_SW && oneSecondPassed)
  {
    menuChanged = true;
    viewChanged = true;
  }
}

void displayHighScore()
{
  if (menuChanged)
  {
    lcd.clear();
    lcd.setCursor(0, 0);

    STRUCT_SCORE score = readHighScore();

    lcd.print(score.playerName);

    lcd.setCursor(0, 1);
    lcd.print(score.score);

    menuChanged = false;
  }
}

void displayInfoMenu()
{
  if (menuChanged)
  {
    lcd.clear();
    lcd.setCursor(0, 0);

    // selected option
    lcd.print(infoMenuStrings[cursorPosition]);

    // next line
    if (cursorPosition <= 2)
    {
      lcd.setCursor(0, 1);

      lcd.print(infoMenuStrings[cursorPosition + 1]);
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print(infoMenuStrings[0]);
    }

    menuChanged = false;
  }
}

void displayMenu()
{
  if (menuChanged)
  {
    lcd.clear();
    lcd.setCursor(1, 0);

    // selected option
    lcd.print(">");
    lcd.print(menuStrings[cursorPosition]);

    // next line
    if (cursorPosition <= 2)
    {
      lcd.setCursor(1, 1);

      lcd.print(menuStrings[cursorPosition + 1]);
    }
    else
    {
      lcd.setCursor(1, 1);

      lcd.print(menuStrings[0]);
    }

    menuChanged = false;
  }
}

void displayGameInfo()
{
  if (gameInfoChanged)
  {
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("Lvl ");
    lcd.print(level);

    lcd.print(" Goal ");
    lcd.print(currentGoal);
    lcd.print("/");
    lcd.print(goal);

    lcd.setCursor(0, 1);

    lcd.print(timeLeft);
    lcd.print("s ");

    lcd.print("Score ");
    lcd.print(gameScore);

    gameInfoChanged = false;
  }
}

void displayGameOverMenu()
{
  if (menuChanged)
  {
    lcd.clear();

    STRUCT_SCORE highestScore = readHighScore();

    lcd.print("Score: ");
    lcd.print(gameScore);

    lcd.setCursor(0, 1);

    if (gameScore <= highestScore.score)
    {
      lcd.print("Better luck next time!");
    }
    else
    {
      lcd.print("You beat the high score!");
    }

    menuChanged = false;
  }
}

void menu()
{
  updateCursor();

  if (viewChanged)
  {
    // go to Info view
    if (currentView == 0 && cursorPosition == 3)
    {
      currentView = 1;
    }
    // go to High Score
    else if (currentView == 0 && cursorPosition == 2)
    {
      currentView = 2;
    }
    // start game
    else if (currentView == 0 && cursorPosition == 0)
    {
      gameStarted = true;
      currentView = 9;
    }
    // return to main menu
    else if (currentView != 0)
    {
      currentView = 0;
    }
    viewChanged = false;
  }

  if (currentView == 0)
  {
    displayMenu();
  }
  else if (currentView == 1)
  {
    displayInfoMenu();
  }
  else if (currentView == 2)
  {
    displayHighScore();
  }
  else if (currentView == 4)
  {
    displayGameOverMenu();
  }
  else if (currentView == 9)
  {
    lcd.clear();
  }
}
