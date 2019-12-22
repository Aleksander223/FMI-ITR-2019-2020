//#include "gameMelody.h"
#include "globals.h"
#include "joystick.h"
#include "lcdMenu.h"
#include "gameScore.h"
#include "gameLoop.h"

// starts debug mode
void debugMode()
{
  Serial.begin(9600);
}

void setup()
{
  // LCD start
  lcd.begin(16, 2);

  // Matrix start
  lc.shutdown(0, false);                       // turn off power saving, enables display
  lc.setIntensity(0, matrixDisplayBrightness); //  sets brightness (0~15 possible values)
  lc.clearDisplay(0);                          // clear screen

  // Pin initializations

  // LCD Brightness
  pinMode(brightnessPin, OUTPUT);

  // Buzzer
  pinMode(buzzerPin, OUTPUT);

  // SFX Buzzer
  pinMode(SFXBuzzerPin, OUTPUT);

  analogWrite(brightnessPin, LCDBrightness);

  // Joystick
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(SWPin, INPUT_PULLUP);

  // Menu
  menu();

  //  debugMode();

  // filter random input
  joyStickState = getJoystick();
}

void loop()
{
  currentTime = millis();

  joyStickState = getJoystick();

  if (!gameStarted)
  {
    menu();
  }
  else if (!gameInitialized)
  {
    // initialize game variables
    initGame();

    gameInitialized = true;
  }
  else
  {
    displayGameInfo();
    gameLoop();
  }
}
