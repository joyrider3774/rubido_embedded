#ifndef COMMONVARS_H
#define COMMONVARS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "defines.h"
#include "Platform.h"
#include "cselector.h"
#include "cmainmenu.h"
#include "savestate.h"

typedef struct CBoardParts CBoardParts;
typedef struct CPeg CPeg;
typedef struct SPoint SPoint;
typedef struct CSelector CSelector;
typedef struct SaveData SaveData;

//the colours of the skin, set by preloadImages
extern uint16_t ColorBackground, ColorForeground, ColorText;

//the images of the skin, set by preloadImages. The full screen ones are run length encoded
extern const uint8_t* imgBackground, *imgCredits, *imgInfoEasy, *imgInfoHard, *imgInfoVeryEasy,
	*imgInfoVeryHard, *imgTitleScreen;
extern const uint8_t* imgCredits1, *imgCredits2, *imgEasy1, *imgHard1, *imgNewGame1, *imgNewGame2,
	*imgPeg, *imgVeryEasy1, *imgVeryHard1;

//game
extern CSelector *GameSelector;
extern bool PrintFormShown;
extern int Moves;
extern int Difficulty;
extern CBoardParts* BoardParts; // boardparts instance that will hold all the boardparts

//titlescreen
extern CMainMenu* Menu;

//main
extern uint8_t prevButtons, currButtons;
extern bool debugMode;
//set when a screen has to be drawn again as a whole, the screens only draw what changed
extern uint8_t needRedraw;
extern int GameState; // the game state
extern SaveData saveData;
#endif
