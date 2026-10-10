#include <stdint.h>
#include "commonvars.h"
#include "helperfuncs.h"
//the one bit pictures of the black & white skin, which the band renderer reads too
#include "onebitimage.h"
//the art read from a card, where a card build gets its rows from
#include "cardimages.h"
//the four bit pictures of the default_4b skin, the background and the peg
#include "fourbitimage.h"

//A row of an image on its way to the display. Where flash is plain memory an evenly placed
//row is handed over where it lies, otherwise it is copied into the scratch row first. A 16
//bit read needs an even address, a core like the Cortex-M0+ faults on an odd one
static inline const uint16_t* ImageRow(const void* src, uint16_t* scratch, int count)
{
#if PLATFORM_DIRECT_FLASH
    if (((uintptr_t)src & 1) == 0)
        return (const uint16_t*)src;
#endif
    PLATFORM_READ_BYTES((uint8_t*)scratch, src, count * sizeof(uint16_t));
    return scratch;
}

//A card build has no art in flash at all: the pictures come off the card while the game runs,
//every skin of them, see cardimages.h. So none of the generated headers are part of it, and
//the sizes are checked against the card's index rather than at compile time
#if !CARDIMAGES
//only the skin FORCESKIN picks is part of the build (a 1 bpp buffer forces the black & white one)
#if FORCESKIN == skinDefault
#include "images/default/background_RLE565.h"
#include "images/default/credits_RLE565.h"
#include "images/default/infoeasy_RLE565.h"
#include "images/default/infohard_RLE565.h"
#include "images/default/infoveryeasy_RLE565.h"
#include "images/default/infoveryhard_RLE565.h"
#include "images/default/titlescreen_RLE565.h"
#include "images/default/credits1_RLE565.h"
#include "images/default/credits2_RLE565.h"
#include "images/default/easy1_RLE565.h"
#include "images/default/hard1_RLE565.h"
#include "images/default/newgame1_RLE565.h"
#include "images/default/newgame2_RLE565.h"
#include "images/default/peg_RGB565_LE.h"
#include "images/default/veryeasy1_RLE565.h"
#include "images/default/veryhard1_RLE565.h"
#endif

#if FORCESKIN == skinBlackWhite
#include "images/black_white/background_RLE565.h"
#include "images/black_white/credits_RLE565.h"
#include "images/black_white/infoeasy_RLE565.h"
#include "images/black_white/infohard_RLE565.h"
#include "images/black_white/infoveryeasy_RLE565.h"
#include "images/black_white/infoveryhard_RLE565.h"
#include "images/black_white/titlescreen_RLE565.h"
#include "images/black_white/credits1_RLE565.h"
#include "images/black_white/credits2_RLE565.h"
#include "images/black_white/easy1_RLE565.h"
#include "images/black_white/hard1_RLE565.h"
#include "images/black_white/newgame1_RLE565.h"
#include "images/black_white/newgame2_RLE565.h"
#include "images/black_white/peg_RGB565_LE.h"
#include "images/black_white/veryeasy1_RLE565.h"
#include "images/black_white/veryhard1_RLE565.h"


#endif

//The same set again from the mixed skin: the background and the peg in sixteen colours and
//every other picture the black & white art, see skinDefault4b in defines.h
#if FORCESKIN == skinDefault4b
#include "images/default_4b/background_RLE565.h"
#include "images/default_4b/credits_RLE565.h"
#include "images/default_4b/infoeasy_RLE565.h"
#include "images/default_4b/infohard_RLE565.h"
#include "images/default_4b/infoveryeasy_RLE565.h"
#include "images/default_4b/infoveryhard_RLE565.h"
#include "images/default_4b/titlescreen_RLE565.h"
#include "images/default_4b/credits1_RLE565.h"
#include "images/default_4b/credits2_RLE565.h"
#include "images/default_4b/easy1_RLE565.h"
#include "images/default_4b/hard1_RLE565.h"
#include "images/default_4b/newgame1_RLE565.h"
#include "images/default_4b/newgame2_RLE565.h"
#include "images/default_4b/peg_RGB565_LE.h"
#include "images/default_4b/veryeasy1_RLE565.h"
#include "images/default_4b/veryhard1_RLE565.h"
#endif

//the game draws the images with the sizes in defines.h, a skin has to keep to them
#if FORCESKIN == skinDefault
#define SKIN_IMAGE(name) default_##name
#elif FORCESKIN == skinDefault4b
#define SKIN_IMAGE(name) default_4b_##name
#else
#define SKIN_IMAGE(name) black_white_##name
#endif
static_assert((SKIN_IMAGE(background_width) == fullScreenWidth) && (SKIN_IMAGE(background_height) == fullScreenHeight) &&
			  (SKIN_IMAGE(credits_width) == fullScreenWidth) && (SKIN_IMAGE(credits_height) == fullScreenHeight) &&
			  (SKIN_IMAGE(infoeasy_width) == fullScreenWidth) && (SKIN_IMAGE(infoeasy_height) == fullScreenHeight) &&
			  (SKIN_IMAGE(infohard_width) == fullScreenWidth) && (SKIN_IMAGE(infohard_height) == fullScreenHeight) &&
			  (SKIN_IMAGE(infoveryeasy_width) == fullScreenWidth) && (SKIN_IMAGE(infoveryeasy_height) == fullScreenHeight) &&
			  (SKIN_IMAGE(infoveryhard_width) == fullScreenWidth) && (SKIN_IMAGE(infoveryhard_height) == fullScreenHeight) &&
			  (SKIN_IMAGE(titlescreen_width) == fullScreenWidth) && (SKIN_IMAGE(titlescreen_height) == fullScreenHeight),
			  "a full screen image of the skin is not 128x128");
static_assert((SKIN_IMAGE(newgame1_width) == newGameWidth) && (SKIN_IMAGE(newgame1_height) == newGameHeight) &&
			  (SKIN_IMAGE(newgame2_width) == newGameWidth) && (SKIN_IMAGE(newgame2_height) == newGameHeight) &&
			  (SKIN_IMAGE(credits1_width) == creditsWordWidth) && (SKIN_IMAGE(credits1_height) == creditsWordHeight) &&
			  (SKIN_IMAGE(credits2_width) == creditsWordWidth) && (SKIN_IMAGE(credits2_height) == creditsWordHeight) &&
			  (SKIN_IMAGE(veryeasy1_width) == veryEasyWidth) && (SKIN_IMAGE(veryeasy1_height) == difficultyHeight) &&
			  (SKIN_IMAGE(easy1_width) == easyWidth) && (SKIN_IMAGE(easy1_height) == difficultyHeight) &&
			  (SKIN_IMAGE(hard1_width) == hardWidth) && (SKIN_IMAGE(hard1_height) == difficultyHeight) &&
			  (SKIN_IMAGE(veryhard1_width) == veryHardWidth) && (SKIN_IMAGE(veryhard1_height) == difficultyHeight),
			  "a menu word image of the skin does not have the size in defines.h");
static_assert((SKIN_IMAGE(peg_width) == TileWidth) && (SKIN_IMAGE(peg_height) == TileHeight * 7),
			  "the peg image of the skin is not 7 tiles of TileWidth x TileHeight");
#endif

//The skin in use. A flash build has the one FORCESKIN put in it and no other; a card build has
//every skin on the card and this is whichever of them is being shown, see CardImages_UseSkin
uint8_t currentSkin(void)
{
#if CARDIMAGES
    return CardImages_Skin();
#else
    return FORCESKIN;
#endif
}

void preloadImages(void)
{
    switch(currentSkin())
    {
#if CARDIMAGES || (FORCESKIN == skinDefault)
        case skinDefault:
            ColorBackground = SCREEN.color565(100,120,255);
            ColorForeground = SCREEN.color565(0,10,255);
            ColorText = SCREEN.color565(0,0,0);
            break;
#endif
#if CARDIMAGES || (FORCESKIN == skinDefault4b)
        case skinDefault4b:
            ColorBackground = SCREEN.color565(56,76,216);
            ColorForeground = SCREEN.color565(0,0,0);
            ColorText = SCREEN.color565(200,200,200);
            //The pictures of this skin that are one bit a pixel, drawn beside the coloured ones.
            //Both are colours the background already holds: it is a field of blues from #2838B8
            //to #384CD8 over black, and #384CD8 is the brightest of them, so the line art reads as
            //the same game rather than as another set of pictures.
            //The blue is the lines and the black the ground, the way the black & white art was
            //drawn: lettering on a dark page rather than a blue one
            ColorOneBitSet = SCREEN.color565(255,255,255);
            ColorOneBitClear = SCREEN.color565(0,0,0);
            break;
#endif
#if CARDIMAGES || (FORCESKIN == skinBlackWhite)
        case skinBlackWhite:
            ColorBackground = SCREEN.color565(0,0,0);
            ColorForeground = SCREEN.color565(255,255,255);
            ColorText = SCREEN.color565(255,255,255);
            ColorOneBitSet = SCREEN.color565(255,255,255);
            ColorOneBitClear = SCREEN.color565(0,0,0);
            break;
#endif
    }
#if CARDIMAGES
    //Every picture comes off the card, by the numbers tools/mkcard.py generated into
    //cardindex.h. The skin is not decided here any more: the card holds all of them
    imgBackground = CardImages_Get(CARD_IMG_BACKGROUND);
    imgCredits = CardImages_Get(CARD_IMG_CREDITS);
    imgInfoEasy = CardImages_Get(CARD_IMG_INFOEASY);
    imgInfoHard = CardImages_Get(CARD_IMG_INFOHARD);
    imgInfoVeryEasy = CardImages_Get(CARD_IMG_INFOVERYEASY);
    imgInfoVeryHard = CardImages_Get(CARD_IMG_INFOVERYHARD);
    imgTitleScreen = CardImages_Get(CARD_IMG_TITLESCREEN);
    imgCredits1 = CardImages_Get(CARD_IMG_CREDITS1);
    imgCredits2 = CardImages_Get(CARD_IMG_CREDITS2);
    imgEasy1 = CardImages_Get(CARD_IMG_EASY1);
    imgHard1 = CardImages_Get(CARD_IMG_HARD1);
    imgNewGame1 = CardImages_Get(CARD_IMG_NEWGAME1);
    imgNewGame2 = CardImages_Get(CARD_IMG_NEWGAME2);
    imgPeg = CardImages_Get(CARD_IMG_PEG);
    imgVeryEasy1 = CardImages_Get(CARD_IMG_VERYEASY1);
    imgVeryHard1 = CardImages_Get(CARD_IMG_VERYHARD1);
#else
    imgBackground = SKIN_IMAGE(background_rle);
    imgCredits = SKIN_IMAGE(credits_rle);
    imgInfoEasy = SKIN_IMAGE(infoeasy_rle);
    imgInfoHard = SKIN_IMAGE(infohard_rle);
    imgInfoVeryEasy = SKIN_IMAGE(infoveryeasy_rle);
    imgInfoVeryHard = SKIN_IMAGE(infoveryhard_rle);
    imgTitleScreen = SKIN_IMAGE(titlescreen_rle);
    imgCredits1 = SKIN_IMAGE(credits1_rle);
    imgCredits2 = SKIN_IMAGE(credits2_rle);
    imgEasy1 = SKIN_IMAGE(easy1_rle);
    imgHard1 = SKIN_IMAGE(hard1_rle);
    imgNewGame1 = SKIN_IMAGE(newgame1_rle);
    imgNewGame2 = SKIN_IMAGE(newgame2_rle);
    imgPeg = SKIN_IMAGE(peg_data);
    imgVeryEasy1 = SKIN_IMAGE(veryeasy1_rle);
    imgVeryHard1 = SKIN_IMAGE(veryhard1_rle);
#endif
}

void fillScreen(uint16_t color)
{
    GFX.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, color);
}

void fillRect(int x, int y, int w, int h, uint16_t color)
{
    GFX.fillRect(x, y, w, h, color);
}

void drawRect(int x, int y, int w, int h, uint16_t color)
{
    GFX.drawRect(x, y, w, h, color);
}

//multi line text in the 6x8 GLCD font, a cell of 6x8 per character with the background only
//painted when it differs from the text colour, 6 pixels per char and 9 per line
void printText(int16_t x, int16_t y, const char* str, uint16_t color, uint16_t bg, uint8_t size)
{
	int16_t cursorX = x;
	int16_t cursorY = y;
	if (!str)
		return;
#if LOVYANGFX
	//LovyanGFX's drawChar that takes the colours hands them to the font the other way
	//round, set them as the text colour instead. Its default font is the same 6x8 GLCD
	//font and a background equal to the text colour is left out here as well
	GFX.setTextColor(color, bg);
	GFX.setTextSize(size);
#endif
#if SCREENBUFFER == 0
	//Straight to the display every character would be a write transaction of its own, and
	//the chip select sits on the I/O expander: that is I2C traffic per character. One
	//transaction for the whole text instead. Into a buffer nothing is sent, so nothing to do
	SCREEN.startWrite();
#endif
	while (*str)
	{
		if (*str == '\n')
		{
			cursorY += 9 * size;
			cursorX = x;
			str++;
			continue;
		}
#if LOVYANGFX
		GFX.drawChar((uint8_t)*str, cursorX, cursorY);
#else
		GFX.drawChar(cursorX, cursorY, *str, color, bg, size);
#endif
		cursorX += 6 * size;
		str++;
	}
#if SCREENBUFFER == 0
	SCREEN.endWrite();
#endif
}

//Draws the w x h part at sx,sy of a raw RGB565 little endian image that is dataWidth pixels
//wide, at x,y on the screen and clipped to it. Every visible row comes out of flash in one
//copy: LovyanGFX reads image data through plain pointers, but PROGMEM on the ESP8266 is flash
//that only takes 32 bit reads, so the rows are read here with PLATFORM_READ_BYTES
#if CARDIMAGES
//the plain call is the keyed one with nothing left out
void drawImagePart(int x, int y, int sx, int sy, int w, int h, const uint8_t* data, int dataWidth)
{
    drawImagePart(x, y, sx, sy, w, h, data, dataWidth, false);
}

void drawImagePart(int x, int y, int sx, int sy, int w, int h, const uint8_t* data, int dataWidth, bool transparent)
#else
void drawImagePart(int x, int y, int sx, int sy, int w, int h, const uint8_t* data, int dataWidth)
#endif
{
#if FOURBITIMAGES
    //A mixed skin: the peg is four bits a pixel and everything else one, and a picture says which
    //it is in its first byte. See FOURBIT_MAGIC and ONEBIT_MAGIC
    if (data && (PLATFORM_READ_BYTE(data) == FOURBIT_MAGIC))
    {
        (void)dataWidth;
        //this one draws every pixel of the part, there is nothing to leave out
        drawImage4BitPart(x, y, sx, sy, w, h, data, false);
        return;
    }
#endif
#if ONEBITIMAGES
    //the skin's pictures carry their own width, the one passed in is the RGB565 path's
    (void)dataWidth;
    drawImageOneBitPart(x, y, sx, sy, w, h, data, false);
#else
    if (!data)
        return;
    //the columns and rows of the part that are on screen
    const int c0 = (x < 0) ? -x : 0;
    const int c1 = (x + w > WINDOW_WIDTH) ? WINDOW_WIDTH - x : w;
    const int r0 = (y < 0) ? -y : 0;
    const int r1 = (y + h > WINDOW_HEIGHT) ? WINDOW_HEIGHT - y : h;
    if ((c0 >= c1) || (r0 >= r1))
        return;
    const int cols = c1 - c0;
    const int dx = x + c0;
    //little endian RGB565 like every device, so the bytes can be copied straight into it
    uint16_t row[WINDOW_WIDTH];
#if SCREENBUFFER
    void* buffer = SCREENBUFFER_PIXELS();
    if (!buffer)
        return;
    for (int r = r0; r < r1; r++)
    {
        const int dy = y + r;
#if CARDIMAGES
        //no flash array to index: the row comes from the arena or off the card, see cardimages.h
        if (!CardImages_Row(data, sx + c0, sy + r, cols, row))
            memset(row, 0, (size_t)cols * sizeof(uint16_t));
#else
        PLATFORM_READ_BYTES((uint8_t*)row, data + ((sy + r) * dataWidth + sx + c0) * sizeof(uint16_t), cols * sizeof(uint16_t));
#endif
  //A card build may be asked to leave the transparent colour out, which for this game is a blue,
  //see COLOR_TRANSPARENT. Such a pixel is simply not written and the buffer keeps what it held,
  //the same way the four bit drawing treats its own key
  #if CARDIMAGES
    #define CARD_KEEPS(c) (transparent && (row[(c)] == COLOR_TRANSPARENT))
  #else
    #define CARD_KEEPS(c) false
  #endif
  #if SCREENBUFFER == 16
        uint16_t* d = &((uint16_t*)buffer)[dy * WINDOW_WIDTH + dx];
        //a 16 bpp sprite keeps its pixels byte swapped
        for (int c = 0; c < cols; c++)
            if (!CARD_KEEPS(c))
                d[c] = (uint16_t)((row[c] >> 8) | (row[c] << 8));
  #elif SCREENBUFFER == 8
        uint8_t* d = &((uint8_t*)buffer)[dy * WINDOW_WIDTH + dx];
        //RGB332, the same conversion SetBufferPixel does
        for (int c = 0; c < cols; c++)
            if (!CARD_KEEPS(c))
                d[c] = ToBuffer332(row[c], (int16_t)(dx + c), (int16_t)dy);
  #else
        for (int c = 0; c < cols; c++)
            if (!CARD_KEEPS(c))
                SetBufferBit((uint8_t*)buffer, dx + c, dy, row[c]);
  #endif
  #undef CARD_KEEPS
    }
#else
#if CARDIMAGES
    //THE BUS RULE: where the card shares the display's bus, reading it takes the bus over, so a
    //row is fetched with nothing of the display's open and only then sent. A window around the
    //whole part cannot survive a card read in the middle of filling it. See Platform_CardRead
    for (int r = r0; r < r1; r++)
    {
        if (!CardImages_Row(data, sx + c0, sy + r, cols, row))
            memset(row, 0, (size_t)cols * sizeof(uint16_t));
        if (!transparent)
        {
            SCREEN.startWrite();
  #if LOVYANGFX
            SCREEN.setAddrWindow(dx, y + r, cols, 1);
            SCREEN.writePixels(row, cols, true);
  #else
            GFX.pushImage(dx, y + r, cols, 1, row);
  #endif
            SCREEN.endWrite();
            continue;
        }
        //the runs between the transparent pixels, each sent on its own: this game's key is a
        //blue (COLOR_TRANSPARENT) and what it covers has to keep what the display shows
        int c = 0;
        while (c < cols)
        {
            while ((c < cols) && (row[c] == COLOR_TRANSPARENT))
                c++;
            const int runX = c;
            while ((c < cols) && (row[c] != COLOR_TRANSPARENT))
                c++;
            if (c == runX)
                continue;
            SCREEN.startWrite();
  #if LOVYANGFX
            SCREEN.setAddrWindow(dx + runX, y + r, c - runX, 1);
            SCREEN.writePixels(row + runX, c - runX, true);
  #else
            GFX.pushImage(dx + runX, y + r, c - runX, 1, row + runX);
  #endif
            SCREEN.endWrite();
        }
    }
#else
    //straight to the display. The chip select sits on the I/O expander, every write
    //transaction costs I2C traffic, so all the rows go out in one
    SCREEN.startWrite();
  #if LOVYANGFX
    //one window for the whole part, filled a row at a time
    SCREEN.setAddrWindow(dx, y + r0, cols, r1 - r0);
  #endif
    for (int r = r0; r < r1; r++)
    {
        const uint16_t* prow = ImageRow(data + ((sy + r) * dataWidth + sx + c0) * sizeof(uint16_t), row, cols);
  #if LOVYANGFX
        //true: the values are plain RGB565, the library puts them in display order
        SCREEN.writePixels(prow, cols, true);
  #else
        GFX.pushImage(dx, y + r, cols, 1, prow);
  #endif
    }
#endif
    SCREEN.endWrite();
#endif
#endif
}

void drawImage(int x, int y, int w, int h, const uint8_t* data)
{
    drawImagePart(x, y, 0, 0, w, h, data, w);
}

//Draws the w x h part at sx,sy of a run length encoded RGB565 image made by tools/png2rle565.py
//that is dataWidth x dataHeight, at x,y on the screen and clipped to it. A control byte with the
//top bit set is a run of (c & 0x7F) + 1 times the pixel after it, otherwise c + 1 literal pixels
//follow. The image can only be read from its start, so it is decoded up to the last row of the
//part and only the pixels of the part are drawn. With transparent set its COLOR_TRANSPARENT pixels
//are skipped. The data is read with PLATFORM_READ_BYTE and PLATFORM_READ_BYTES: LovyanGFX reads
//image data through plain pointers, but PROGMEM on the ESP8266 is flash that only takes 32 bit reads
void drawImageRLEPart(int x, int y, int sx, int sy, int w, int h, const uint8_t* data, int dataWidth, int dataHeight, bool transparent)
{
#if CARDIMAGES
    //Nothing on the card is run length encoded: a card read is by offset and the game wants a row
    //or a strip out of the middle of a picture, which only unpacked pixels allow. What is passed
    //in is a card picture and not a stream to decode - handing it to the decoder below drew noise
    (void)dataWidth;
    (void)dataHeight;
    drawImagePart(x, y, sx, sy, w, h, data, 0, transparent);
    return;
#endif
#if FOURBITIMAGES
    //A mixed skin: the background is four bits a pixel and the other full screen pictures one,
    //and a picture says which it is in its first byte. See FOURBIT_MAGIC and ONEBIT_MAGIC
    if (data && (PLATFORM_READ_BYTE(data) == FOURBIT_MAGIC))
    {
        (void)dataWidth;
        (void)dataHeight;
        drawImage4BitPart(x, y, sx, sy, w, h, data, transparent);
        return;
    }
#endif
#if ONEBITIMAGES
    //the skin's pictures carry their own size, the ones passed in are the RGB565 path's
    (void)dataWidth;
    (void)dataHeight;
    drawImageOneBitPart(x, y, sx, sy, w, h, data, transparent);
#else
    if (!data || (w <= 0) || (h <= 0))
        return;
    //where the image's top left corner lands on the screen
    const int ox = x - sx;
    const int oy = y - sy;
    //the columns and rows of the image that are drawn: the part, cut to the image and the screen
    int c0 = sx, c1 = sx + w, r0 = sy, r1 = sy + h;
    if (c0 < 0) c0 = 0;
    if (r0 < 0) r0 = 0;
    if (c1 > dataWidth) c1 = dataWidth;
    if (r1 > dataHeight) r1 = dataHeight;
    if (c0 < -ox) c0 = -ox;
    if (r0 < -oy) r0 = -oy;
    if (c1 > WINDOW_WIDTH - ox) c1 = WINDOW_WIDTH - ox;
    if (r1 > WINDOW_HEIGHT - oy) r1 = WINDOW_HEIGHT - oy;
    if ((c0 >= c1) || (r0 >= r1))
        return;
#if SCREENBUFFER
    void* dst = SCREENBUFFER_PIXELS();
    if (!dst)
        return;
#else
    //the chip select sits on the I/O expander, so the whole part goes out in one transaction
    //and, without transparency, one window. LovyanGFX's pushBlock and pushPixels open one of
    //their own per call, there the write variants are used inside this one
    SCREEN.startWrite();
    if (!transparent)
        SCREEN.setAddrWindow(ox + c0, oy + r0, c1 - c0, r1 - r0);
#endif
    //a control covers at most 128 pixels
    uint16_t pixels[128];
    uint32_t left = (uint32_t)dataWidth * dataHeight;
    int cx = 0, cy = 0;
    //decoding stops after the last row of the part
    while ((left > 0) && (cy < r1))
    {
        uint8_t control = PLATFORM_READ_BYTE(data++);
        uint16_t count = (control & 0x7F) + 1;
        if (count > left)
            count = (uint16_t)left;
        const bool run = (control & 0x80) != 0;
        uint16_t color = 0;
        if (run)
        {
            color = PLATFORM_READ_BYTE(data) | (PLATFORM_READ_BYTE(data + 1) << 8);
            data += 2;
        }
        else
        {
            //pixels that all lie in rows above the part are only skipped, a control that
            //reaches into the part is read
            if ((uint32_t)(cy * dataWidth + cx + count - 1) / dataWidth >= (uint32_t)r0)
                PLATFORM_READ_BYTES((uint8_t*)pixels, data, count * sizeof(uint16_t));
            data += count * sizeof(uint16_t);
        }
        left -= count;
        //the pixels of the control a row at a time, only the part is drawn
        for (uint16_t done = 0; done < count; )
        {
            int n = dataWidth - cx;
            if (n > count - done)
                n = count - done;
            if ((cy >= r0) && (cy < r1))
            {
                const int a = (cx > c0) ? cx : c0;
                const int e = (cx + n < c1) ? cx + n : c1;
                if (a < e)
                {
                    const uint16_t* src = &pixels[done + (a - cx)];
#if SCREENBUFFER
                    const int sy2 = oy + cy;
                    for (int c = a; c < e; c++)
                    {
                        const uint16_t pixel = run ? color : src[c - a];
                        if (!transparent || (pixel != COLOR_TRANSPARENT))
                            SetBufferPixel(dst, ox + c, sy2, pixel);
                    }
#else
                    //without transparency the pixels fill the one window in order, with it every
                    //run of opaque pixels goes out in a window of its own
                    int c = a;
                    while (c < e)
                    {
                        int start = c;
                        if (transparent)
                        {
                            while ((c < e) && ((run ? color : src[c - a]) == COLOR_TRANSPARENT))
                                c++;
                            start = c;
                            while ((c < e) && ((run ? color : src[c - a]) != COLOR_TRANSPARENT))
                                c++;
                            if (start == c)
                                continue;
                            SCREEN.setAddrWindow(ox + start, oy + cy, c - start, 1);
                        }
                        else
                            c = e;
  #if LOVYANGFX
                        //a uint16_t colour is taken as plain RGB565, true: so are the pixels
                        if (run)
                            SCREEN.writeColor(color, c - start);
                        else
                            SCREEN.writePixels(&src[start - a], c - start, true);
  #else
                        if (run)
                            SCREEN.pushBlock(color, c - start);
                        else
                            SCREEN.pushPixels((uint16_t*)&src[start - a], c - start);
  #endif
                    }
#endif
                }
            }
            done += n;
            cx += n;
            if (cx == dataWidth)
            {
                cx = 0;
                cy++;
            }
        }
    }
#if !SCREENBUFFER
    SCREEN.endWrite();
#endif
#endif
}

void drawImageRLE(int x, int y, int w, int h, const uint8_t* data)
{
    drawImageRLEPart(x, y, 0, 0, w, h, data, w, h, false);
}

void drawImageRLETransparent(int x, int y, int w, int h, const uint8_t* data)
{
    drawImageRLEPart(x, y, 0, 0, w, h, data, w, h, true);
}

//the game's background is as big as the screen, so the part at x,y is the part that belongs there
void drawBackgroundPart(int x, int y, int w, int h)
{
    drawImageRLEPart(x, y, x, y, w, h, imgBackground, fullScreenWidth, fullScreenHeight, false);
}
