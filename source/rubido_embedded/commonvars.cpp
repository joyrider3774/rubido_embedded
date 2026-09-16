#include <stdint.h>
#include "commonvars.h"
#include "cboardparts.h"
#include "cselector.h"
#include "cmainmenu.h"
#include "savestate.h"

uint16_t ColorBackground, ColorForeground, ColorText;

const uint8_t* imgBackground, *imgCredits, *imgInfoEasy, *imgInfoHard, *imgInfoVeryEasy,
	*imgInfoVeryHard, *imgTitleScreen;
const uint8_t* imgCredits1, *imgCredits2, *imgEasy1, *imgHard1, *imgNewGame1, *imgNewGame2,
	*imgPeg, *imgVeryEasy1, *imgVeryHard1;

//game
CSelector *GameSelector;
bool PrintFormShown = false;
CBoardParts* BoardParts; // boardparts instance that will hold all the boardparts
int Difficulty = VeryEasy;
int Moves = 0;


//titlescreen
CMainMenu* Menu;

//main
uint8_t prevButtons, currButtons;
bool debugMode = false;
uint8_t needRedraw = 1;
int GameState = GSTitleScreenInit; // the game state
SaveData saveData;
