/*
 * Code that detects knocks form a passive buzzer. After 5 seconds
 * an alarm (in the form of a buzzer tone) will be started. If a 
 * button is pressed it will be reset.
 */


// Pin declarations
const int knockDetectorPin = A5;
const int alarmLEDPin = 2;
const int alarmBuzzerPin = 9;
const int resetButtonPin  = 4;

// Values used for main loop
int buzzerValue = 0;
int buttonValue = 0;
int ledValue = LOW;

bool alarmStarted = false;

unsigned long startTime = 0;
unsigned long currentTime = 0;
unsigned long interval = 5000; // 5 seconds

void startAlarmCountdown() {
  alarmStarted = true;
  startTime = currentTime;

  // LED that is on if the countdown and alarm are active
  ledValue = HIGH;
}

void resetAlarm() {
  noTone(alarmBuzzerPin);
  alarmStarted = false;

  // show that the alarm is inactive and ready to be tripped
  ledValue = LOW;
}

void setup() {
    pinMode(knockDetectorPin, INPUT);
    pinMode(alarmLEDPin, OUTPUT);
    pinMode(alarmBuzzerPin, OUTPUT);
    pinMode(resetButtonPin, INPUT_PULLUP);
}

void loop() {
  buzzerValue = analogRead(knockDetectorPin);
  buttonValue = digitalRead(resetButtonPin);

  currentTime = millis();

  // detect knocks
  if(buzzerValue && !alarmStarted){
    startAlarmCountdown();
  }
  
  // start alarm if 5s passed
  if(alarmStarted && startTime + interval < currentTime) {
    tone(alarmBuzzerPin, 400);
  } 

  if(!buttonValue){
    resetAlarm();
  }

  digitalWrite(alarmLEDPin, ledValue);
}
