#include "cmainmenu.h"
#include "commonvars.h"
#include "helperfuncs.h"
#include "sound.h"

//where the menu words are
#define newGameX (WINDOW_WIDTH / 2 - newGameWidth / 2)
#define newGameY (64-10)
#define creditsWordX (WINDOW_WIDTH / 2 - creditsWordWidth / 2)
#define creditsWordY (64+10)

//the selection the menu words on screen show, 0 when they have to be drawn again
static int shownSelection = 0;

CMainMenu* CMainMenu_Create()
{
	CMainMenu* Result = (CMainMenu*)malloc(sizeof(CMainMenu));
    Result->Selection = 1;

	return Result;
}

 // return the current selection
int CMainMenu_GetSelection(CMainMenu* MainMenu)
{
	return MainMenu->Selection;
}

// Destructor will free the surface images
void CMainMenu_Destroy(CMainMenu* MainMenu)
{
    free(MainMenu);
    MainMenu = NULL;
}

// Increase the selection if it goes to far set i to the first selection
void CMainMenu_NextItem(CMainMenu* MainMenu)
{
    MainMenu->Selection++;
    if (MainMenu->Selection == 3)
        MainMenu->Selection = 1;

    playMenuSelectSound();
}

// decrease the selection if it goes to low set it to the last selection
void CMainMenu_PreviousItem(CMainMenu* MainMenu)
{
    MainMenu->Selection--;
    if (MainMenu->Selection == 0)
        MainMenu->Selection = 2;
    playMenuSelectSound();
}

// Draw the main menu, only what changed since the last time
void CMainMenu_Draw(CMainMenu* MainMenu)
{
    if (needRedraw)
    {
        needRedraw = 0;
        // draw the title screen background
        drawImageRLE(0, 0, fullScreenWidth, fullScreenHeight, imgTitleScreen);
        shownSelection = 0;
    }
    else if (MainMenu->Selection != shownSelection)
    {
        // the words are transparent, the title screen under them is drawn again first
        drawImageRLEPart(newGameX, newGameY, newGameX, newGameY, newGameWidth, newGameHeight, imgTitleScreen, fullScreenWidth, fullScreenHeight, false);
        drawImageRLEPart(creditsWordX, creditsWordY, creditsWordX, creditsWordY, creditsWordWidth, creditsWordHeight, imgTitleScreen, fullScreenWidth, fullScreenHeight, false);
    }

    if (MainMenu->Selection == shownSelection)
        return;
    shownSelection = MainMenu->Selection;

    // if selection = newgame draw the purple new game image in the middle of the screen
    if (MainMenu->Selection == 1)
    {
        drawImageRLETransparent(newGameX, newGameY, newGameWidth, newGameHeight, imgNewGame1);
    }
    else
    // draw the blue one
    {
        drawImageRLETransparent(newGameX, newGameY, newGameWidth, newGameHeight, imgNewGame2);
    }

    if (MainMenu->Selection == 2) // = credits draw purpe
    {
        drawImageRLETransparent(creditsWordX, creditsWordY, creditsWordWidth, creditsWordHeight, imgCredits1);
    }
    else // blue
    {
        drawImageRLETransparent(creditsWordX, creditsWordY, creditsWordWidth, creditsWordHeight, imgCredits2);
    }
}
