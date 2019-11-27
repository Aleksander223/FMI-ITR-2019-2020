#include <LiquidCrystal.h>
#include <EEPROM.h>



const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

// game settings
int startingLevelValue = 0;
int currentLevel = 0;
int score = 0;
int highestScore = 0;

char playerName[] = "AAA";

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// joystick pins
const int xAxisPin = A0;
const int yAxisPin = A1;
const int swButtonPin = A2;

// joystick values
int xAxis = 0;
int yAxis = 0;
int swButton = 0;

// joystick functions
const int minDeadzone = 400;
const int maxDeadzone = 800;

bool upActive, downActive, leftActive, rightActive, buttonActive;

bool checkLeft() {
  if (xAxis > minDeadzone && xAxis < maxDeadzone) {
    leftActive = false;
  }
  
  if (xAxis < minDeadzone && !leftActive) {
    leftActive = true;
    return true;
  }

  return false;
}

bool checkUp() {
  if (yAxis > minDeadzone && yAxis < maxDeadzone) {
    upActive = false;
  }
  
  if (yAxis < minDeadzone && !upActive) {
    upActive = true;
    return true;
  }

  return false;
}

bool checkDown() {
  if (yAxis > minDeadzone && yAxis < maxDeadzone) {
    downActive = false;
  }
  
  if (yAxis > maxDeadzone && !downActive) {
    downActive = true;
    return true;
  }

  return false;
}

bool checkRight() {
  if (xAxis > minDeadzone && xAxis < maxDeadzone) {
    rightActive = false;
  }
  
  if (xAxis > maxDeadzone && !rightActive) {
    rightActive = true;
    return true;
  }

  return false;
}

bool checkButton() {
  if (swButton) {
    buttonActive = false;
  }

  if (!swButton && !buttonActive) {
    buttonActive = true;
    return true;
  }

  return false;
}

// menu boilerplate

struct cursorPositions {
  int col;
  int row;
};

class View{
private:
  bool isMenu;
  int options;
  cursorPositions* positions;
  int cursorPosition;
  void (*displayFunction)();
  
public:
  // we are limiting the callback functions to 4
  // it ain't pretty but neither is dynamically allocating an array of functions pointers
  // i will refactor this some time
  void (*callbacks[4])();
  bool changesOptions[4];
  
  View(bool menu, int noOfOptions, cursorPositions* pos, void(*func)()) {
    isMenu = menu;
    options = noOfOptions;
    positions = pos;
    displayFunction = func;
    
    cursorPosition = 0;

    for(int i = 0; i < 4; i++) {
      changesOptions[i] = false;
    }
  }
  
  void displayView() {
    lcd.clear();
    displayFunction();

    if(isMenu) {
      cursorPositions aux = positions[cursorPosition];
      lcd.setCursor(aux.col, aux.row);
      lcd.print(">");
    }
  }

  void next() {
    if(isMenu) {
      cursorPosition++;
      if (cursorPosition >= options) {
        cursorPosition = 0;
    }
      lcd.clear();
    }
  }

  void back() {
    if(isMenu) {
      cursorPosition--;
      if (cursorPosition < 0) {
        cursorPosition = options - 1;
      }
      lcd.clear();
    }
  }

  void onClick() {
    callbacks[cursorPosition]();
  }

  bool changesOption() {
    return changesOptions[cursorPosition];
  }
};

/* 0123456789012345
 * >Play >Settings
 * >Highscores
  */
void displayMainMenu() {
  lcd.setCursor(1, 0);
  lcd.print("Play");

  lcd.setCursor(7, 0);
  lcd.print("Settings");

  lcd.setCursor(1, 1);
  lcd.print("Highscore");
}

/* 0123456789012345
 * >Name: 
 * >Level: x   Back
  */

void displaySettings() {
  lcd.setCursor(1, 0);
  lcd.print("Name: ");
  lcd.print(playerName);
  
  lcd.setCursor(1, 1);
  lcd.print("Level: ");
  lcd.print(startingLevelValue);

  lcd.setCursor(12, 1);
  lcd.print("Back");
}

/* 0123456789012345
 * Lives: 3
 * Level: 
  */

void displayPlay() {
  lcd.setCursor(0, 0);
  lcd.print("Lives: 3");
  
  lcd.setCursor(0, 1);
  lcd.print("Level: ");
  lcd.print(currentLevel);
}

/* 0123456789012345
 * Highest Score
 * AAA     10 
  */

void displayHighScore() {
  lcd.setCursor(0, 0);
  lcd.print("Highest Score");

  lcd.setCursor(0, 1);
  lcd.print(playerName);

  lcd.setCursor(8, 1);
  lcd.print(highestScore);
}

// cursors
cursorPositions menuCursors[] = {
  {
    0, 0
  },
  {
    6, 0
  },
  {
    0, 1
  }
};
cursorPositions settingsCursors[] = {
  {
    0, 0
  },
  {
    0, 1
  },
  {
    11, 1
  }
};


// defining views
View mainMenu(true, 3, menuCursors, displayMainMenu);
View settings(true, 3, settingsCursors, displaySettings);
View play(false, 0, NULL, displayPlay);
View highScore(false, 0, NULL, displayHighScore);

View* views[] = {&mainMenu, &settings, &play, &highScore};
int currentView = 0;


bool menuHasUpdated = false;

bool changingOption = false;
bool gameOn = false;
bool gameOver = false;

// timers
int levelUpdateInterval = 5000;
int maxGameLength = 11000;

unsigned long lastUpdateTime = 0; // last update to level
unsigned long startTime = 0; // game start time
unsigned long currentTime = 0;

void gameLoop() {
  currentTime = millis();

  if(lastUpdateTime + levelUpdateInterval < currentTime) {
    currentLevel++;
    // play has updated
    play.displayView();

    lastUpdateTime = currentTime;
  }

  if(startTime + maxGameLength < currentTime) {
    score = currentLevel * 3;
    currentLevel = startingLevelValue;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game finished.");
    lcd.setCursor(0, 1);
    lcd.print("Press to exit.");

    gameOver = true;

    if(score > highestScore) {
      highestScore = score;
      EEPROM.write(0, highestScore);
    }
  }
}

void updateView() {
    if(!gameOn) {
      if(checkRight() && !changingOption) {
      views[currentView]->next();
      menuHasUpdated = true;
      }

      if(checkLeft() && !changingOption) {
        views[currentView]->back();
        menuHasUpdated = true;
      }
  
      if(checkButton()) {
        views[currentView]->onClick();
        menuHasUpdated = true;
  
        if(views[currentView]->changesOption()) {
          changingOption = !changingOption;
        }
      }
  
      if(changingOption) {
        views[currentView]->onClick();
      }
  
      if(menuHasUpdated) {
        views[currentView]->displayView();
        menuHasUpdated = false;
      } 
    } else {
      if(gameOver) {
        if(checkButton()) {
          currentView = 0;
          menuHasUpdated = true;
          gameOver = false;
          gameOn= false;
        }
      } else {
        gameLoop();
      }
    }
}

// callbacks
void goToMenu() {
  currentView = 0;
}

void goToSettings() {
  currentView = 1;
}

void goToHighScore() {
  currentView = 3;
}

void goToPlay() {
  currentView = 2;
  currentLevel = startingLevelValue;
  gameOn = true;
  gameOver = false;

  // update game start time
  startTime = millis();
  lastUpdateTime = startTime;
}

void changeLevel() { 
  if(checkUp() && startingLevelValue < 9) {
    startingLevelValue++;
    menuHasUpdated = true;
  }
  if(checkDown() && startingLevelValue > 0) {
    startingLevelValue--;
    menuHasUpdated = true;
  }
}
void setup() {
  // get high score
  highestScore = (int) EEPROM.read(0);
  
  lcd.begin(16, 2);

  // allocate callbacks
  // menu
  mainMenu.callbacks[1] = goToSettings;
  mainMenu.callbacks[0] = goToPlay;
  mainMenu.callbacks[2] = goToHighScore;
  // settings
  settings.callbacks[2] = goToMenu;
  settings.callbacks[1] = changeLevel;
  settings.changesOptions[1] = true;
  // play
  play.callbacks[0] = gameLoop;
  // highscore
  highScore.callbacks[0] = goToMenu;
  

  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(swButtonPin, INPUT_PULLUP);

  views[currentView]->displayView();;

  Serial.begin(9600);
}

void loop() {
  xAxis = analogRead(xAxisPin);
  yAxis = analogRead(yAxisPin);
  swButton = digitalRead(swButtonPin);

  
  updateView();  
//  Serial.print(xAxis); Serial.print("   "); Serial.print(yAxis); Serial.print("   "); Serial.println(swButton);
}
