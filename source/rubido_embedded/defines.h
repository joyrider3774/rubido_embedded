#ifndef defines_h
#define defines_h

//the device comes first: the display library and SCREENBUFFER are device settings, see
//PlatformESPboy.h / PlatformSDL.h
#include "PlatformDevice.h"

// window size, the ESPboy display
#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 128

// The diffrent difficultys
#define VeryEasy 0
#define Easy 1
#define Hard 2
#define VeryHard 3

// The diffrent gameStates possible in the game
#define GSQuit 0
#define GSGame 1
#define GSTitleScreen 2
#define GSDifficultySelect 3
#define GSCredits 4

#define GSInitDiff 50

#define GSGameInit (GSGame + GSInitDiff)
#define GSTitleScreenInit (GSTitleScreen + GSInitDiff)
#define GSDifficultySelectInit (GSDifficultySelect + GSInitDiff)
#define GSCreditsInit (GSCredits + GSInitDiff)

//game defines
#define NrOfRows 9
#define NrOfCols 9
#define TileWidth 13
#define TileHeight 13
#define IDPeg 1
#define XOffSet 4
#define YOffSet 5

//the sizes of the images, the same in every skin (helperfuncs.cpp checks them)
#define fullScreenWidth 128
#define fullScreenHeight 128
#define newGameWidth 62
#define newGameHeight 12
#define creditsWordWidth 44
#define creditsWordHeight 11
#define veryEasyWidth 108
#define easyWidth 84
#define hardWidth 80
#define veryHardWidth 107
#define difficultyHeight 12

//the colour the menu words use for transparent pixels, RGB565 of (0,11,255)
#define COLOR_TRANSPARENT 0x005F

#define skinDefault 0
#define skinBlackWhite 1

//FORCESKIN: -1 = the default skin, or the black & white one with a 1 bpp buffer, n = skin n
//(0 default, 1 black & white). There is no skin option in the game, so only the skin used is
//built in. A 1 bpp buffer can only show the black & white skin, so it forces that one.
//Set by the device header or the build
#if SCREENBUFFER == 1
  #if defined(FORCESKIN) && (FORCESKIN >= 0) && (FORCESKIN != skinBlackWhite)
  #error "a 1 bpp buffer can only show the black & white skin, FORCESKIN has to be -1 or 1"
  #endif
  #undef FORCESKIN
  #define FORCESKIN skinBlackWhite
#elif !defined(FORCESKIN) || (FORCESKIN < 0)
  #undef FORCESKIN
  #define FORCESKIN skinDefault
#endif

#define FRAMERATE 30
//1 = every frame waits until 1/FRAMERATE of a second has passed, 0 = a frame starts as soon
//as the last one is done, to see how fast the game can go. The music counts frames, so
//without the lock it plays faster as well. A build can set it itself
#ifndef FPSLOCK
#define FPSLOCK 1
#endif
//1 = the debug header (frame rate, free heap and stack) is always shown, Up + Down does not
//hide it. 0 = it starts hidden and Up + Down shows and hides it. A build can set it itself
#ifndef FORCEDEBUG
#define FORCEDEBUG 0
#endif

#endif
