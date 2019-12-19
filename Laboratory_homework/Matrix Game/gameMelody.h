#pragma once

#include "buzzerPitches.h"
#include "globals.h"

// Song declaration
int melody[] = {
    NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4,
    NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_A4, NOTE_A4,

    // part 1
    NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_C4, NOTE_A3, NOTE_GS3, NOTE_B3,
    NOTE_E4, NOTE_C4, NOTE_D4, NOTE_B3, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_A4, NOTE_GS4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 4, 4, 4, 4,
    6, 8, 4, 8, 8, 6, 8, 4, 8, 8, 4, 8, 8, 4, 4, 4, 4, 4,

    // part 2
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 4, 4, 4, 4, 1};

byte maxNotes = 55;
byte currentNote = 0;

unsigned long lastNoteTime = 0;
int interval = 0;
bool notePlaying = false;

// This plays a sound effect
void playSFX(int t)
{
  tone(SFXBuzzerPin, t, 100);
}

// This plays the Korobeiniki song asynchronously using millis()
void playMelody()
{
  if (!musicOn)
  {
    return;
  }
  // go back to the beggining
  if (currentNote >= maxNotes)
  {
    currentNote = 0;
  }

  // play a note
  if (!notePlaying)
  {
    int noteDuration = 1000 / noteDurations[currentNote];
    tone(buzzerPin, melody[currentNote], noteDuration);
    notePlaying = true;

    // space between notes
    interval = noteDuration * 1.30;
    lastNoteTime = currentTime;
  }

  // next note
  if (currentTime > lastNoteTime + interval)
  {
    notePlaying = false;
    noTone(buzzerPin);

    currentNote++;
  }
}
