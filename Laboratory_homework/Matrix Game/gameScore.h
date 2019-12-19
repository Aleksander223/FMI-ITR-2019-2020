#pragma once

#include <EEPROM.h>
#include <string.h>

// makes returning score easier
typedef struct
{
  char playerName[4];
  unsigned short score;
} STRUCT_SCORE;

STRUCT_SCORE readHighScore()
{
  STRUCT_SCORE tmp;

  EEPROM.get(0, tmp);

  return tmp;
}

void writeHighScore(const char *player, unsigned short score)
{
  STRUCT_SCORE tmp;
  strncpy(tmp.playerName, player, 4);
  tmp.score = score;

  EEPROM.put(0, tmp);
}
