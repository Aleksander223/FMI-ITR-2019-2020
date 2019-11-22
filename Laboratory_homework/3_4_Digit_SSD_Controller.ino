// joystick pins
const int joyX = A0;
const int joyY = A1;
const int joySW = A2;

// 4 digit seven segment display pins
const int pinE = 3;
const int pinD = 4;
const int pinDP = 5;
const int pinC = 6;
const int pinG = 7;
const int pinD4 = 8;
const int pinD1 = 9;
const int pinA = 10;
const int pinF = 11;
const int pinD2 = 12;
const int pinD3 = 13;
const int pinB = 2;


// 4D SSD Code
const int segSize = 8;

const int noOfDisplays = 4;
const int noOfDigits = 10;

int dpState = LOW;

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int digits[noOfDisplays] = {
    pinD1, pinD2, pinD3, pinD4
};
byte digitMatrix[noOfDigits][segSize - 1] = {
// a b c d e f g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1} // 9
};

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
      digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  digitalWrite(segments[segSize - 1], decimalPoint);
}

void clearDisplay(byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
      digitalWrite(segments[i], LOW);
  }

  digitalWrite(segments[segSize - 1], decimalPoint);
}

void showDigit(int num) {
  for(int i = 0; i < noOfDisplays; i++){
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

//////////////////////////

// joystick values
int valueX, valueY, valueSW;

// joystick code
int minDeadzone = 350, maxDeadzone = 750;
bool leftModified, rightModified = false, upModified = false, downModified = false;

bool checkLeft() {
  if(valueY > minDeadzone && valueY < maxDeadzone) {
    leftModified = false;
  }
  
  if(valueY < minDeadzone && !leftModified) {
    leftModified = true;
    return true;
  }

  return false;
}

bool checkRight() {
  if(valueY > minDeadzone && valueY < maxDeadzone) {
    rightModified = false;
  }
  
  if(valueY > maxDeadzone && !rightModified) {
    rightModified = true;
    return true;
  }

  return false;
}

bool checkDown() {
  if(valueX > minDeadzone && valueX < maxDeadzone) {
    downModified = false;
  }
  
  if(valueX < minDeadzone && !downModified) {
    downModified = true;
    return true;
  }

  return false;
}

bool checkUp() {
  if(valueX > minDeadzone && valueX < maxDeadzone) {
    upModified = false;
  }
  
  if(valueX > maxDeadzone && !upModified) {
    upModified = true;
    return true;
  }

  return false;
}

bool leftMoved, rightMoved, upMoved, downMoved;

int counter = 0;
int numbers[] = {0, 0, 0, 0};

// for showing digits
int digitShowInterval = 5;
unsigned long currentTime = 0, startTime = 0;
int digitCounter = 0;

// for blinking active digit
int blinkInterval = 500;
unsigned long blinkStart = 0;
bool blinkState = true;

// if digit is locked
bool locked = false; // initially none are locked
bool buttonPressed = false;

void setup() {
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joySW, INPUT_PULLUP);

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinDP, OUTPUT);
  pinMode(pinD1, OUTPUT);
  pinMode(pinD2, OUTPUT);
  pinMode(pinD3, OUTPUT);
  pinMode(pinD4, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {

  currentTime = millis();

  // get values from joystick
  valueX = analogRead(joyX);
  valueY = analogRead(joyY);
  valueSW = digitalRead(joySW);


  // check for movements on the joystick
  leftMoved = checkLeft();
  rightMoved = checkRight();
  upMoved = checkUp();
  downMoved = checkDown();

  // check if we lock a digit
  if (!valueSW && !buttonPressed) {
    locked = !locked;
    buttonPressed = true;
  } else if(valueSW && buttonPressed) {
    buttonPressed = false;
  }

  // if we move right or left we change the active display
  if(rightMoved && !locked) {
    counter = (counter + 1) % noOfDisplays;
  } else if(leftMoved && !locked) {
    counter = counter - 1;
    if (counter < 0) {
      counter = noOfDisplays - 1;
    }
  }

  // if we move up or down we change the value on the active display
  if(upMoved && locked) {
    numbers[counter] = (numbers[counter] + 1) % noOfDigits;
  } else if(downMoved && locked) {
    numbers[counter] = numbers[counter] - 1;

    if(numbers[counter] < 0){
      numbers[counter] = noOfDigits - 1;
    }
  }

  // non-blocking show digits
  if(currentTime > startTime + digitShowInterval) {
    startTime = currentTime;

    digitCounter = (digitCounter + 1) % noOfDisplays;
  }

  // blink
  if(currentTime > blinkStart + blinkInterval) {
    blinkStart = currentTime;
    blinkState = !blinkState;
  }

  showDigit(digitCounter);
  // don't directly show the active digit since we will blink it
  if(digitCounter != counter) {
    displayNumber(numbers[digitCounter], LOW);
  } else if(!locked) {
    // if selected but not locked
    if(blinkState) {
      displayNumber(numbers[digitCounter], locked);
    } else {
      clearDisplay(locked);
    }
  } else {
    displayNumber(numbers[digitCounter], locked);
  }
}
