/* Globals header
 *  This makes it easier to manage the globals
 */

#pragma once

// Libraries
#include <LiquidCrystal.h>
#include <LedControl.h>

// Pin Declarations

// LCD Screen
const int RS = 7;
const int enable = 8;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int brightnessPin = 9;

// Matrix Display
const int DIN = 12;
const int CLK = 11;
const int LOAD = 10;

// Buzzer
const int buzzerPin = 6;
const int SFXBuzzerPin = 13;

// Joystick
const int xAxisPin = A0;
const int yAxisPin = A1;
const int SWPin = A2;

// Object declarations

// LCD Screen
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// Matrix display
LedControl lc = LedControl(DIN, CLK, LOAD, 1);

// GLOBALS

// hardware
byte matrixDisplayBrightness = 2;
byte LCDBrightness = 90;

// timers
unsigned long currentTime = 0;

// input
byte joyStickState = -1;

// DIFFICULTY
const byte startingLevel = 1;
const byte startingGoal = 2;
const byte levelGoalUpdate = 1;
const byte startingTimeLimit = 75;

const int startingFallInterval = 900;
const int startingGracePeriod = 400;
const int startingSpawnDelay = 600;

const int spawnDelayMin = 50;
const int fallIntervalMin = 100;

const int spawnDelayDecrement = 50;
const int fallIntervalDecrement = 150;

// game info variables
byte level = startingLevel;
byte goal = startingGoal;
byte currentGoal = 0;
byte timeLimit = startingTimeLimit;
int timeLeft = timeLimit;
unsigned int gameScore = 0;
bool gameInfoChanged = true;


// booleans
bool gameStarted = false;
bool musicOn = false;
bool gameInitialized = false;

// menu
bool menuChanged = true;
bool viewChanged = false;

char playerName[4] = "AAA";

// sounds stupid but the arduino somehow senses the joystick is pressed upon initialization so this is necessary
// would use delay but that's against the rules ¯\_(ツ)_/¯
bool oneSecondPassed = false;
