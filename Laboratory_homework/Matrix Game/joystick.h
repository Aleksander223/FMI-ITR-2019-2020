#pragma once

#include "globals.h"

// joystick states
#define JOY_UP 0
#define JOY_DOWN 1
#define JOY_LEFT 2
#define JOY_RIGHT 3
#define JOY_SW 4

// deadzones
const int minDeadzone = 300;
const int maxDeadzone = 700;

bool upActive = false;
bool downActive = false;
bool leftActive = false;
bool rightActive = false;
bool swActive = false;

int xAxis, yAxis, joySW;

// individual directions, with state checking
bool checkUp()
{
  if (yAxis > minDeadzone && yAxis < maxDeadzone)
  {
    upActive = false;
  }

  if (yAxis < minDeadzone && !upActive)
  {
    upActive = true;
    return true;
  }

  return false;
}

bool checkDown()
{
  if (yAxis > minDeadzone && yAxis < maxDeadzone)
  {
    downActive = false;
  }

  if (yAxis > maxDeadzone && !downActive)
  {
    downActive = true;
    return true;
  }

  return false;
}

bool checkLeft()
{
  if (xAxis > minDeadzone && xAxis < maxDeadzone)
  {
    leftActive = false;
  }

  if (xAxis < minDeadzone && !leftActive)
  {
    leftActive = true;
    return true;
  }

  return false;
}

bool checkRight()
{
  if (xAxis > minDeadzone && xAxis < maxDeadzone)
  {
    rightActive = false;
  }

  if (xAxis > maxDeadzone && !rightActive)
  {
    rightActive = true;
    return true;
  }

  return false;
}

bool checkButton()
{
  if (joySW)
  {
    swActive = false;
  }

  if (!joySW && !swActive)
  {
    swActive = true;
    return true;
  }

  return false;
}

// joystick input
int getJoystick()
{
  // get the joystick values
  xAxis = analogRead(xAxisPin);
  yAxis = analogRead(yAxisPin);
  joySW = !digitalRead(SWPin);

  if (checkUp())
  {
    //    Serial.println("UP");
    return JOY_UP;
  }
  else if (checkDown())
  {
    //    Serial.println("DOWN");
    return JOY_DOWN;
  }
  else if (checkLeft())
  {
    //    Serial.println("LEFT");
    return JOY_LEFT;
  }
  else if (checkRight())
  {
    //    Serial.println("RIGHT");
    return JOY_RIGHT;
  }
  else if (checkButton())
  {
    //    Serial.println("BUTTON");
    return JOY_SW;
  }

  // no input
  return -1;
}
