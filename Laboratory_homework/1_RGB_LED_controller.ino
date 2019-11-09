/* Author: Adam Alexandru-Vlad
 * 1st Homework for ITR 2018-2019
 * For licensing see LICENCE.MD
 * For more details see README.MD
 */ 

// Potentiometers
const int knobRed = A0;
const int knobGreen = A1;
const int knobBlue = A2;

// LEDs
const int ledRed = 11;
const int ledGreen = 10;
const int ledBlue = 9;

// Values of the potentiometers
int potRed = 0;
int potGreen = 0;
int potBlue = 0;

// Values of the color channels
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  // Potentiometer
  pinMode(knobRed, INPUT);
  pinMode(knobGreen, INPUT);
  pinMode(knobBlue, INPUT);

  // RGB Led
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);

//  Serial.begin(9600);
}

void loop() {
  // we read the values from the potentiometer
  potRed = analogRead(knobRed);
  potGreen = analogRead(knobGreen);
  potBlue = analogRead(knobBlue);

  // we map them
  redValue = map(potRed, 0, 1023, 0, 255);
  greenValue = map(potGreen, 0, 1023, 0, 255);
  blueValue = map(potBlue, 0, 1023, 0, 255);

//  Serial.println(redValue);
//  Serial.println(greenValue);
//  Serial.println(blueValue);

  // we write the values to the LED
  analogWrite(ledRed, redValue);
  analogWrite(ledGreen, greenValue);
  analogWrite(ledBlue, blueValue);
}
