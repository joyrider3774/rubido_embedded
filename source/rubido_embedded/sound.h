#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define musNone 0
#define musStart 1
#define musWinner 2
#define musLoser 3


void playSelectSound();
void playErrorSound();
void playGameAction();
void playMenuSelectSound();
void playMenuBackSound();
void playMenuAcknowlege();

void initSound();
void SelectMusic(uint8_t musicFile, uint8_t loop);
void initMusic();
void playTickSound();
void setMusicOn(uint8_t value);
void setSoundOn(uint8_t value);
void musicTimer();
void processSound();
uint8_t isMusicOn();
uint8_t isSoundOn();

#endif