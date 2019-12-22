#pragma once

#include "globals.h"
#include "joystick.h"
#include "gameScore.h"

/*
 * I lost most of this because I reformatted without the code being on git, so I have only redone 
 * and bodged the necessary parts. I apologize in advance if this is spaghetti.
 */

byte cursorPosition = 0;
// this is for the settings menu
byte cursorHorizontal = 0;
const byte numStrings = 4;
const byte numSettings = 2;
byte currentView = 0;

bool settingChanged = false;

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

char settingsStrings[numStrings][8] = {
    "Name: ",
    "Level: "};

void updateCursor(int limit)
{
  if (joyStickState == JOY_DOWN)
  {
    cursorPosition = (cursorPosition + 1) % limit;
    menuChanged = true;
  }
  else if (joyStickState == JOY_UP)
  {
    if (cursorPosition == 0)
    {
      cursorPosition = limit - 1;
    }
    else
    {
      cursorPosition--;
    }
    menuChanged = true;
  }
  else if (joyStickState == JOY_SW && cursorHorizontal == 0)
  {
    menuChanged = true;
    viewChanged = true;
  }
  else if (joyStickState == JOY_SW && !settingChanged)
  {
    settingChanged = true;
  }
  else if (currentView == 5 && joyStickState == JOY_LEFT)
  {
    if (cursorHorizontal)
    {
      cursorHorizontal--;
    }

    menuChanged = true;
  }
  else if (currentView == 5 && joyStickState == JOY_RIGHT)
  {
    if (cursorHorizontal < 3)
    {
      cursorHorizontal++;
    }

    menuChanged = true;
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

void displayGameOver()
{
  if (menuChanged)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(playerName);
    lcd.print(" ");
    lcd.print(gameScore);

    STRUCT_SCORE score = readHighScore();

    lcd.setCursor(0, 1);
    // didnt beat highscore
    if (score.score > gameScore)
    {
      lcd.print("Try harder!");
    }
    else
    {
      lcd.print("U beat hiscore!");

      // set the score
      writeHighScore(playerName, gameScore);
    }

    menuChanged = false;
  }
}

void displaySettings()
{
  if (menuChanged || settingChanged)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Name: ");
    lcd.print(playerName);

    lcd.setCursor(0, 1);
    lcd.print("Level: ");
    lcd.print(startingLevel);

    // name
    if (cursorPosition == 0 && cursorHorizontal != 0)
    {
      lcd.setCursor(cursorHorizontal + 5, cursorPosition);

      if (settingChanged)
      {
        playerName[cursorHorizontal - 1]++;
      }

      if (playerName[cursorHorizontal - 1] > 'Z')
      {
        playerName[cursorHorizontal - 1] = 'A';
      }
    }
    // level
    else if (cursorPosition == 1 && cursorHorizontal != 0)
    {
      lcd.setCursor(cursorHorizontal + 6, cursorPosition);

      // to avoid wrapping
      cursorHorizontal = 1;

      if (settingChanged)
      {
        startingLevel++;
      }

      if (startingLevel > 9)
      {
        startingLevel = 1;
      }
    }
    else
    {
      lcd.setCursor(cursorHorizontal, cursorPosition);
    }
    lcd.cursor();

    menuChanged = false;
    if (settingChanged)
    {
      menuChanged = true;
      settingChanged = false;
    }
  }
}

void displayReplayMenu()
{
  if (menuChanged)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Replay");

    lcd.setCursor(0, 1);
    lcd.print("Back to menu");

    lcd.cursor();
    lcd.setCursor(0, cursorPosition);

    menuChanged = false;
  }
}

void menu()
{
  if (currentView == 1 || currentView == 0)
  {
    // for main menu and info
    updateCursor(numStrings);
  }
  else
  {
    // for settings and replay menu
    updateCursor(numSettings);
  }

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
    // go to settings
    else if (currentView == 0 && cursorPosition == 1)
    {
      currentView = 5;
    }
    // game over -> replay menu
    else if (currentView == 3)
    {
      currentView = 4;
    }
    // replay menu
    else if (currentView == 4)
    {
      // replay
      if (cursorPosition == 0)
      {
        currentView = 9;
        gameStarted = true;
      }
      // back to menu
      else
      {
        currentView = 0;
        lcd.noCursor();
      }
    }
    // return to main menu
    else if (currentView != 0)
    {
      currentView = 0;
      lcd.noCursor();
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
  else if (currentView == 3)
  {
    displayGameOver();
  }
  else if (currentView == 4)
  {
    displayReplayMenu();
  }
  else if (currentView == 5)
  {
    displaySettings();
  }
  else if (currentView == 9)
  {
    lcd.clear();
  }
}
