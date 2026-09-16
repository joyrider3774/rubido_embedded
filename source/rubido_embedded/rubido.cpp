#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sound.h"
#include "cboardparts.h"
#include "cmainmenu.h"
#include "cpeg.h"
#include "cselector.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "savestate.h"

// the info texts on the game screen: 2 lines of at most 5 characters of the 6x8 font
#define infoWidth (5 * 6)
#define infoHeight (9 + 8)

void resetGlobals()
{
	PrintFormShown = false;
	Difficulty = VeryEasy;
	Moves = 0;
	GameState = GSTitleScreenInit;
}

void setupGame()
{
	resetGlobals();
	loadSavedState();
	BoardParts = CBoardParts_Create();
	Menu = CMainMenu_Create();
	GameSelector = CSelector_Create(4,4);
}

void terminateGame()
{
	CBoardParts_Destroy(BoardParts);
	CMainMenu_Destroy(Menu);
	CSelector_Destroy(GameSelector);
	saveSavedState();
}

// procedure that calculates how many moves are possible in the current board state
// we can simply do this by checking all parts and see if they can move to all directions
// the canmoveto method in CPegs is does all the checking
int MovesLeft()
{
    CPeg *BoardPart ;
    int TmpMoves=0,Y,X;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            BoardPart = CBoardParts_GetPart(BoardParts, X,Y);
            // if there is a boardpart on that X,Y Coordinate
            // check all direction if we can move to that if so increases the movesleft
            if (BoardPart)
            {
                if (CPeg_CanMoveTo(BoardPart,X+2,Y,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X,Y-2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X+2,Y-2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X+2,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y+2,false))
                    TmpMoves++;
                if (CPeg_CanMoveTo(BoardPart,X-2,Y-2,false))
                    TmpMoves++;
            }
        }
    return TmpMoves;
}

// Procedure that counts how many pegs are left on the board (animphase must be lower then 2 io a red or blue peg)
int PegsLeft()
{
    CPeg *BoardPart ;
    int Pegs=0,Y,X;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            BoardPart = CBoardParts_GetPart(BoardParts,X,Y);
            if (BoardPart)
                if ((CPeg_GetType(BoardPart) == IDPeg) && (CPeg_GetAnimPhase(BoardPart) < 2))
                    Pegs++;
        }
    return Pegs;
}

// procedure that draws a boxed message over the playfield

void PrintForm(const char *msg)
{
	const int x = 4;
	const int y = 30;
	const int w = 120;
	const int h = 62;
	PrintFormShown = true;
	fillRect(x,y,w,h,ColorBackground);
	drawRect(x,y,w,h,ColorForeground);
	drawRect(x+1,y+1,w-1,h-1,ColorForeground);
	printText(x+4,y+4,msg,ColorForeground,ColorForeground,1);
}

// this will ceate the initial board state, io a cross of pegs, with the middle on being empty (=animphase 6)
void InitBoard()
{
	CBoardParts_RemoveAll(BoardParts);
    int X,Y;
    for (Y=0;Y < NrOfRows;Y++)
        for (X=0;X < NrOfCols;X++)
        {
            if ((Y < 3) && (X > 2) && (X < 6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
             if ((Y > 2) && (Y <6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
            if ((Y > 5) && (X > 2) && (X <6))
                CBoardParts_Add(BoardParts,CPeg_Create(X,Y));
        }
     CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts,4,4), 6);

}

// Checks if we won the game
bool IsWinningGame()
{
    if (PegsLeft() == 1) //must be 1 peg left
    {
        if ((Difficulty == VeryHard) || (Difficulty == Easy))
        {
            if(CPeg_GetAnimPhase(CBoardParts_GetPart(BoardParts,4,4)) < 2) // must be in the middle with veryhard or easy
                return true;
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;
}

// What is on the game screen right now. Only what differs from the game's state is drawn again, so
// straight to the display (SCREENBUFFER 0) nothing flickers and little is sent. With a buffer the
// buffer keeps the last frame, so the same holds there. needRedraw draws everything again
static int8_t shownPhase[NrOfCols][NrOfRows]; // -1 = the peg is not drawn
static SPoint shownSelector;
static bool selectorShown;
static int shownMovesLeft, shownMoves, shownPegsLeft, shownBest; // -1 = not shown
static bool formShown;
// counted again whenever a peg was drawn
static int movesLeftCount, pegsLeftCount;

// an info text, value -1 shows none. Returns if it was drawn
static bool DrawInfo(int x, int y, const char* label, int value, int* shown)
{
	if (value == *shown)
		return false;
	drawBackgroundPart(x, y, infoWidth, infoHeight);
	if (value >= 0)
	{
		char Msg[16];
		snprintf(Msg, sizeof(Msg), "%s\n%d", label, value);
		printText(x, y, Msg, ColorText, ColorText, 1);
	}
	*shown = value;
	return true;
}

// Brings the game screen up to the game's state
static void DrawGame()
{
	bool drawn = false;
	bool pegsDrawn = false;
	int X, Y;

	if (needRedraw)
	{
		needRedraw = 0;
		// the bare background shows no peg, selector or text, what is there is drawn on it below
		drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgBackground);
		memset(shownPhase, -1, sizeof(shownPhase));
		selectorShown = false;
		shownMovesLeft = shownMoves = shownPegsLeft = shownBest = -1;
		formShown = false;
		drawn = true;
	}

	SPoint position = CSelector_GetPosition(GameSelector);
	bool drawSelector = !selectorShown;
	if (selectorShown && ((position.X != shownSelector.X) || (position.Y != shownSelector.Y)))
	{
		// the selector sticks out a pixel over the tiles around it, the background goes back
		// where it was and the pegs around it are drawn again below
		drawBackgroundPart(XOffSet - 1 + shownSelector.X * TileWidth, YOffSet - 1 + shownSelector.Y * TileHeight, TileWidth + 2, TileHeight + 2);
		for (Y = shownSelector.Y - 1; Y <= shownSelector.Y + 1; Y++)
			for (X = shownSelector.X - 1; X <= shownSelector.X + 1; X++)
				if ((X >= 0) && (X < NrOfCols) && (Y >= 0) && (Y < NrOfRows))
					shownPhase[X][Y] = -1;
		drawSelector = true;
		drawn = true;
	}

	for (int Teller = 0; Teller < BoardParts->ItemCount; Teller++)
	{
		CPeg* Peg = BoardParts->Items[Teller];
		X = CPeg_GetPlayFieldX(Peg);
		Y = CPeg_GetPlayFieldY(Peg);
		if (CPeg_GetAnimPhase(Peg) != shownPhase[X][Y])
		{
			CPeg_Draw(Peg);
			shownPhase[X][Y] = (int8_t)CPeg_GetAnimPhase(Peg);
			pegsDrawn = true;
			drawn = true;
			// a peg next to the selector was drawn over its edge
			if ((abs(X - position.X) <= 1) && (abs(Y - position.Y) <= 1))
				drawSelector = true;
		}
	}

	if (drawSelector)
	{
		CSelector_Draw(GameSelector);
		shownSelector = position;
		selectorShown = true;
		drawn = true;
	}

	if (pegsDrawn)
	{
		movesLeftCount = MovesLeft();
		pegsLeftCount = PegsLeft();
	}

	// Write some info to the screen
	drawn |= DrawInfo(2, 87, "Mvs L", movesLeftCount, &shownMovesLeft);
	drawn |= DrawInfo(2, 105, "Mvs", Moves, &shownMoves);
	drawn |= DrawInfo(86, 87, "Pegs", pegsLeftCount, &shownPegsLeft);
	// Only show best pegs if it isn't 0
	drawn |= DrawInfo(86, 105, "Best", (saveData.BestPegsLeft[Difficulty] != 0) ? (int)saveData.BestPegsLeft[Difficulty] : -1, &shownBest);

	// the form lies over everything else, it is drawn again when something under it was
	if (PrintFormShown && (!formShown || drawn))
	{
		// if it's the winning game show the form with the winning message
		if (IsWinningGame())
		{
			PrintForm("Congrats you have\nsolved the puzzle!\nTry a new\ndifficulty!\n\n(A) to continue");
		}
		else // show the loser messager
		{
			PrintForm("You could not\nsolve the puzzle!\nDo not give up,\ntry it again!\n\n(A) to continue");
		}
	}
	else if (!PrintFormShown && formShown)
		needRedraw = 1;
	formShown = PrintFormShown;
}

void GameInit()
{
	CSelector_SetPosition(GameSelector, 4, 4);
	InitBoard();
    Moves = 0;
	SelectMusic(musNone, 0);
	SelectMusic(musStart, 0);
	PrintFormShown = false;
	needRedraw = 1;
}

// The main Game Loop
void Game()
{
	if (GameState == GSGameInit)
	{
		GameInit();
		GameState -= GSInitDiff;
	}

	if((currButtons & BUTTON_LEFT) && !(prevButtons & BUTTON_LEFT))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X - 1, CSelector_GetPosition(GameSelector).Y);

	if((currButtons & BUTTON_RIGHT) && !(prevButtons & BUTTON_RIGHT))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X + 1, CSelector_GetPosition(GameSelector).Y);

	if((currButtons & BUTTON_UP) && !(prevButtons & BUTTON_UP))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y - 1);

	if((currButtons & BUTTON_DOWN) && !(prevButtons & BUTTON_DOWN))
		if (!PrintFormShown)
			CSelector_SetPosition(GameSelector, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y + 1);


	if((currButtons & BUTTON_B) && !(prevButtons & BUTTON_B))
	{
		GameState = GSTitleScreenInit;
		PrintFormShown = false;
		playMenuBackSound();
	}

	if((currButtons & BUTTON_A) && !(prevButtons & BUTTON_A))
	{
		if (PrintFormShown)
		{
			GameState = GSTitleScreenInit;
			PrintFormShown = false;
			playMenuAcknowlege();
		}
		else
		{
			if (GameSelector->HasSelection) // if we have a selection
			{
				// see if the selected boardpart can move to the current position
				if (CPeg_CanMoveTo(CBoardParts_GetPart(BoardParts, CSelector_GetSelection(GameSelector).X, CSelector_GetSelection(GameSelector).Y), CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y, true))
				{
					playGameAction();
					//if so play a sound, increase the moves, set the selected part to empty and the current part to red
					Moves++;
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetSelection(GameSelector).X, CSelector_GetSelection(GameSelector).Y), 6);
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y), 0);
					// if no moves are left see if the best pegs left value for the current difficulty is
					// greater if so set te new value
					if (MovesLeft() == 0)
					{
						if (saveData.BestPegsLeft[Difficulty] != 0)
						{
							if (PegsLeft() < saveData.BestPegsLeft[Difficulty])
								saveData.BestPegsLeft[Difficulty] = PegsLeft();
						}
						else
							saveData.BestPegsLeft[Difficulty] = PegsLeft();
						saveSavedState();
						// if it's the winning game play the winning sound and show the form with the winning message
						if (IsWinningGame())
						{
							SelectMusic(musNone, 0);
							SelectMusic(musWinner, 0);
							PrintFormShown = true;
						}
						else // show the loser messager, play loser sound
						{
							SelectMusic(musNone, 0);
							SelectMusic(musLoser, 0);
							PrintFormShown = true;
						}
					}

				}
				else // if we can't move to the spot, play the wrong move sound, and reset the selection to a red peg (instead of blue / selected)
				{
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetSelection(GameSelector).X, CSelector_GetSelection(GameSelector).Y), 0);
					playErrorSound();
				}
				CSelector_DeSelect(GameSelector); // deselect the selection
			}
			else // we didn't have a selection, set the new selection
			{
				if (CPeg_GetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y)) == 0)
				{
					playGameAction();
					CPeg_SetAnimPhase(CBoardParts_GetPart(BoardParts, CSelector_GetPosition(GameSelector).X, CSelector_GetPosition(GameSelector).Y), 1);
					CSelector_Select(GameSelector);
				}

			}
		}
	}

	// nothing to draw when the game was left
	if (GameState == GSGame)
		DrawGame();
}

void TitleScreenInit()
{
	needRedraw = 1;
}

// main title screen loop
void TitleScreen()
{
	if(GameState == GSTitleScreenInit)
	{
		TitleScreenInit();
		GameState -= GSInitDiff;
	}

	if((currButtons & BUTTON_DOWN) && !(prevButtons & BUTTON_DOWN))
	{
		CMainMenu_NextItem(Menu);
	}
	if((currButtons & BUTTON_UP) && !(prevButtons & BUTTON_UP))
	{
		CMainMenu_PreviousItem(Menu);
	}
	if((currButtons & BUTTON_A) && !(prevButtons & BUTTON_A))
	{
		playMenuAcknowlege();
		switch (CMainMenu_GetSelection(Menu))
		{
			case 1:
				GameState = GSDifficultySelectInit;
				break;
			case 2:
				GameState = GSCreditsInit;
				break;
		}
	}
	// nothing to draw when the title screen was left
	if (GameState == GSTitleScreen)
		CMainMenu_Draw(Menu);
}

// the difficulty the difficulty screen shows
static int shownDifficulty;

void DifficultySelectInit()
{
	needRedraw = 1;
}

// Main difficulty select loop
void DifficultySelect()
{
	if(GameState == GSDifficultySelectInit)
	{
		DifficultySelectInit();
		GameState -= GSInitDiff;
	}


	if((currButtons & BUTTON_B) && !(prevButtons & BUTTON_B))
	{
		GameState = GSTitleScreenInit;
		playMenuBackSound();
	}

	if((currButtons & BUTTON_A) && !(prevButtons & BUTTON_A))
	{
		playMenuAcknowlege();
		GameState = GSGameInit;
	}

	if((currButtons & BUTTON_LEFT) && !(prevButtons & BUTTON_LEFT))
	{
		playMenuSelectSound();
		if (Difficulty == VeryHard)
		{
			Difficulty = Hard;
		}
		else
			if (Difficulty == Hard)
			{
				Difficulty = Easy;
			}
			else
				if (Difficulty == Easy)
				{
					Difficulty = VeryEasy;
				}
				else
					if (Difficulty == VeryEasy)
						Difficulty = VeryHard;
	}

	if((currButtons & BUTTON_RIGHT) && !(prevButtons & BUTTON_RIGHT))
	{
		playMenuSelectSound();
		if (Difficulty == VeryEasy)
		{
			Difficulty = Easy;
		}
		else
			if (Difficulty == Easy)
			{
				Difficulty = Hard;
			}
			else
				if (Difficulty == Hard)
				{
					Difficulty = VeryHard;
				}
				else
					if (Difficulty == VeryHard)
						Difficulty = VeryEasy;
	}

	// nothing to draw when the difficulty screen was left, or when it already shows this difficulty
	if ((GameState != GSDifficultySelect) || (!needRedraw && (Difficulty == shownDifficulty)))
		return;
	needRedraw = 0;
	shownDifficulty = Difficulty;

	// decide what we draw based on the difficuly
	switch(Difficulty)
	{
		case VeryEasy:
			drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgInfoVeryEasy);
			drawImageRLETransparent(WINDOW_WIDTH / 2 - veryEasyWidth / 2, 19, veryEasyWidth, difficultyHeight, imgVeryEasy1);
			break;
		case Easy:
			drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgInfoEasy);
			drawImageRLETransparent(WINDOW_WIDTH / 2 - easyWidth / 2, 19, easyWidth, difficultyHeight, imgEasy1);
		break;
		case Hard:
			drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgInfoHard);
			drawImageRLETransparent(WINDOW_WIDTH / 2 - hardWidth / 2, 19, hardWidth, difficultyHeight, imgHard1);
			break;
		case VeryHard:
			drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgInfoVeryHard);
			drawImageRLETransparent(WINDOW_WIDTH / 2 - veryHardWidth / 2, 19, veryHardWidth, difficultyHeight, imgVeryHard1);
			break;

	}
}

void CreditsInit()
{
	needRedraw = 1;
}

//Main Credits loop, will just show an image and wait for a button to be pressed
void Credits()
{
	if (GameState == GSCreditsInit)
	{
		CreditsInit();
		GameState -= GSInitDiff;
	}

	if(((currButtons & BUTTON_A) && !(prevButtons & BUTTON_A)) ||
		((currButtons & BUTTON_B) && !(prevButtons & BUTTON_B)))
		{
			playMenuAcknowlege();
			GameState = GSTitleScreenInit;
		}
	if ((GameState == GSCredits) && needRedraw)
	{
		needRedraw = 0;
		drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgCredits);
	}
}

void mainLoop(void)
{
    switch(GameState)
    {
		case GSGameInit:
        case GSGame :
            Game();
            break;
		case GSTitleScreenInit:
        case GSTitleScreen:
            TitleScreen();
            break;
		case GSDifficultySelectInit:
        case GSDifficultySelect:
            DifficultySelect();
            break;
		case GSCreditsInit:
        case GSCredits:
            Credits();
            break;
		default:
			break;
    }
}
