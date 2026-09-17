# Rubido Embedded Version
![DownloadCountTotal](https://img.shields.io/github/downloads/joyrider3774/rubido_embedded/total?label=total%20downloads&style=plastic) ![DownloadCountLatest](https://img.shields.io/github/downloads/joyrider3774/rubido_embedded/latest/total?style=plastic) ![LatestVersion](https://img.shields.io/github/v/tag/joyrider3774/rubido_embedded?label=Latest%20version&style=plastic) ![License](https://img.shields.io/github/license/joyrider3774/rubido_embedded?style=plastic)

Rubido is a little chinese checkers or solitaire game with four difficulties.

## Devices
Every [release](https://github.com/joyrider3774/rubido_embedded/releases) has a build for every device. `releases/` is where a build of your own puts them, it is not part of the repository:

| Device | File | How to install |
| ------ | ---- | -------------- |
| [ESPboy](https://www.espboy.com/) | ESPboy_Rubido.bin | flash it, the board is a LOLIN(WEMOS) D1 mini |
| [Gamebuino META](https://gamebuino.com/gamebuino-meta) | GamebuinoMeta_Rubido.bin | copy it into a folder on the SD card, the .hex is for flashing it directly |
| [Adafruit PyBadge](https://www.adafruit.com/product/4200) | PyBadge_Rubido.uf2 | double press reset and copy it onto the drive that appears |
| [Adafruit PyGamer](https://www.adafruit.com/product/4242) | PyGamer_Rubido.uf2 | same as the PyBadge |
| [Pimoroni PicoSystem](https://shop.pimoroni.com/products/picosystem) | PicoSystem_Rubido.uf2 | hold X while switching on and copy it onto the drive that appears |
| [Pimoroni Explorer](https://shop.pimoroni.com/products/explorer?variant=42092697845843) | Explorer_Rubido.uf2 | hold BOOT while pressing RESET and copy it onto the drive that appears |
| [Pimoroni Tufty 2350](https://shop.pimoroni.com/products/tufty-2350?variant=55811986227579) | Tufty_Rubido.uf2 | hold HOME while pressing RESET and copy it onto the drive that appears |
| [TinyCircuits Thumby Color](https://tinycircuits.com/products/thumby-color) | ThumbyColor_Rubido.uf2 | put it into bootloader mode and copy it onto the RPI-RP2 drive that appears |
| [Playdate](https://play.date/) | Playdate_Rubido.pdx.zip | unzip it and sideload Rubido.pdx, the same pdx runs in the Playdate simulator |
| [Libretro / RetroArch](https://www.retroarch.com/) | Libretro_Rubido.zip | copy rubido_libretro.dll into RetroArch's cores folder and rubido_libretro.info into its info folder, then Load Core and Start Core |
| [Game Boy Advance](https://en.wikipedia.org/wiki/Game_Boy_Advance) | GBA_Rubido.gba | put it on a flash cart or open it in an emulator, the best pegs left are saved in the cartridge's SRAM |
| [PlayStation Portable](https://en.wikipedia.org/wiki/PlayStation_Portable) | PSP_Rubido.PBP | rename it to EBOOT.PBP and put it in ms0:/PSP/GAME/Rubido/ on the memory stick, or open it in PPSSPP |
| [PlayStation Vita](https://en.wikipedia.org/wiki/PlayStation_Vita) | Vita_Rubido.vpk | install it with VitaShell on a Vita with homebrew enabled, or open it in Vita3K |
| Windows | Windows_Rubido.exe | runs on its own, the best pegs left are saved next to it in Rubido.sav |

`python tools/build_releases.py` builds all of them, `python tools/convert_skins.py` turns the images in `assets/skins` into the headers the game includes. The Playdate build also needs the Playdate SDK, see `playdate/CMakeLists.txt`, the libretro core libretro-common, see `libretro/CMakeLists.txt`, the Game Boy Advance build devkitARM and libgba, see `gba/CMakeLists.txt`, the PSP build the pspdev toolchain, see `psp/CMakeLists.txt` (pspdev has no Windows build, so on Windows it is built from WSL), and the Vita build VitaSDK, see `vita/CMakeLists.txt`.

### Buttons
The game's buttons on every device:

| Device | D-pad | A | B |
| ------ | ----- | - | - |
| ESPboy | d-pad | ACT | ESC |
| Gamebuino META | d-pad | A | B |
| Adafruit PyBadge | d-pad | A | B |
| Adafruit PyGamer | joystick | A | B |
| Pimoroni PicoSystem | d-pad | A | B |
| Pimoroni Explorer | A up, C down, B left, Y right | X | Z |
| Pimoroni Tufty 2350 | UP up, DOWN down, A left, C right | B | HOME |
| TinyCircuits Thumby Color | d-pad | A | B |
| Playdate | d-pad | A | B |
| Libretro | d-pad | A | B |
| Game Boy Advance | d-pad | A | B |
| PlayStation Portable | d-pad or the analog stick | Cross | Circle |
| PlayStation Vita | d-pad or the left stick | Cross | Circle |
| Windows | arrow keys | X | C |

On the Tufty 2350 a tap of HOME is B when it is let go. It has no speaker, the game is silent there. Holding RESET until the rear LEDs are dark puts it to sleep, a front button wakes it up again, with UP and DOWN held as well it goes into shipping mode instead.

The Thumby Color's display is 128x128, the game's own size, so it is shown 1:1 over the whole screen. That build has not been tried on the device itself yet.

The Playdate shows the black & white skin, scaled up in the middle of its display.

The Game Boy Advance shows the game scaled to 160x160 in the middle of its screen, with black bars at the sides.

On the PlayStation Portable the game is doubled to 256x256 in the middle of the display, and the high scores are saved next to the EBOOT.PBP in Rubido.sav.

On the PlayStation Vita the game is blown up four times to 512x512 in the middle of the display, and the high scores are saved in ux0:data/Rubido/Rubido.sav.

On the Gamebuino META holding HOME for a second goes back to its loader.

## Playing the Game:
The aim of the game in chinese checkers is to select a (white) peg on the board and jump over another (white) peg to land on an empty (black) spot. When doing this the peg you jumped over will be removed from the board.
You need to play the game in such a way that only one peg remains on the board at the end. Depending on the difficulty you had chosen this can be either (only) in the middle of the board or anywhere on the board.
Also depending on the difficulty you had chosen you can either jump horizontally and veritically over pegs or diagonally as well.

## Diffuclties 

### Very Easy
- Jump over Pegs vertically, horizontally and diagonally
- Last Peg can be anywhere on the board

### Easy
- Jump over Pegs vertically, horizontally and diagonally
- Last Peg must end on the middle board

### Hard
- Jump over Pegs vertically and horizontally only
- Last Peg can be anywhere on the board

### Very Hard
- Jump over Pegs vertically and horizontally only
- Last Peg must end on the middle board

## Controls

| Button | Action |
| ------ | ------ |
| Left | Left in difficulties screen. During gameplay move the peg selector left. |
| Right | Right in difficulties screen. During gameplay move the peg selector Right. |
| Up | Up in main menu screen. During gameplay move the peg selector Up. |
| Down | Down in main menu screen. During gameplay move the peg selector Down. |
| A | Confirm in menu and difficulty selector. During gameplay activate the peg where the peg selector is. If there was a peg already selected it will deselect it |
| B | return to titlescreen |
| (A) + Left + Down | Show or hide the debug info |

## Credits
- Graphics are made by me willems davy aka joyrider3774 using gimp
- Thanks to Roman for sending me an [espboy](https://www.espboy.com/)
