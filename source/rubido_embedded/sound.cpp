#include <stdint.h>
#include <string.h>
#include "sound.h"
#include "commonvars.h"

//constexpr: the tables below are kept in flash, so they have to be worked out when compiling
constexpr float musModifier = (60.0f / 45.0f);
constexpr float sfxSustain = (100.0f * 15.0f / 18.0f);

uint8_t music_note, music_tempo, music_loop, music_on, sound_on, prev_music;
uint8_t selecting_music;
uint16_t music_length;
//points straight at the PLATFORM_PROGMEM tables, they are read with PLATFORM_READ_WORD
const uint16_t* musicArray = nullptr;

// Winner
const uint16_t PLATFORM_PROGMEM music_winner[] = {
    523,  (uint16_t)(100.0f / musModifier),
    659,  (uint16_t)(100.0f / musModifier),
    783,  (uint16_t)(100.0f / musModifier),
    1046, (uint16_t)(300.0f / musModifier),
    1318, (uint16_t)(500.0f / musModifier),
    0, 0
};

// Loser
const uint16_t PLATFORM_PROGMEM music_loser[] = {
    392,  (uint16_t)(200.0f / musModifier),
    369,  (uint16_t)(200.0f / musModifier),
    329,  (uint16_t)(300.0f / musModifier),
    293,  (uint16_t)(300.0f / musModifier),
    277,  (uint16_t)(500.0f / musModifier),
    0, 0
};

// Start
const uint16_t PLATFORM_PROGMEM music_start[] = {
    784,  (uint16_t)(150.0f / musModifier),
    523,  (uint16_t)(150.0f / musModifier),
    659,  (uint16_t)(200.0f / musModifier),
    1047, (uint16_t)(300.0f / musModifier),
    0, 0
};

void setMusicOn(uint8_t value)
{
    music_on = value;
    if (!music_on)
        Platform_StopTone();
}


void SelectMusic(uint8_t musicFile, uint8_t loop)
{
    if (prev_music != musicFile)
    {
        selecting_music = 1;
		prev_music = musicFile;
		musicArray = nullptr;
		music_length = 0;
		switch (musicFile)
		{
			case musStart:
				musicArray = music_start;
				music_length = sizeof(music_start) / sizeof(music_start[0]);
                break;
            case musWinner:
				musicArray = music_winner;
				music_length = sizeof(music_winner) / sizeof(music_winner[0]);
				break;
            case musLoser:
				musicArray = music_loser;
				music_length = sizeof(music_loser) / sizeof(music_loser[0]);
				break;
			default:
				Platform_StopTone();
				break;
		}
		music_note = 0;
		music_tempo = 0;
		music_loop = loop;
        selecting_music = 0;
    }
}

void playNote()
{
    if(musicArray && (music_note + 1 < music_length))
    {
        Platform_PlayTone(PLATFORM_READ_WORD(&musicArray[music_note]), 0);

        //Set the new delay to wait, the tables are in milliseconds
        music_tempo = PLATFORM_READ_WORD(&musicArray[music_note + 1]) * 60/1000/(60/FRAMERATE) ;

        //Skip to the next note
        music_note += 2;

        if (music_note > music_length - 1)
        {
            if(music_loop)
            {
                music_note = 0;
            }
            else
            {
                Platform_StopTone();
            }
        }
    }
}


void musicTimer()
{
    //for nintendo systems as sega one checks it earlier
    if (selecting_music)
    {
        return;
    }

    //Play some music
    if (music_tempo == 0)
    {
        if(music_on)
        {
            playNote();
        }
    }
    //Else wait for the next note to play
    else
    {
        music_tempo--;
    }
}

void initMusic()
{
	prev_music = 0;
	music_note = 0;
	music_length = 0;
	music_tempo = 0;
	music_loop = 0;
	musicArray = nullptr;
	//set to 1 so nothing plays until a music was selected
	selecting_music = 1;
}

void setSoundOn(uint8_t value)
{
    sound_on = value;
}

uint8_t isMusicOn()
{
    return music_on;
}

uint8_t isSoundOn()
{
    return sound_on;
}

void initSound()
{
    sound_on = 0;
}

static void playSound(uint16_t freq)
{
    if (sound_on)
    {
        Platform_PlayTone(freq, (uint16_t)sfxSustain);
    }
}

void playSelectSound(void)
{
    playSound(1250);
}


void playErrorSound(void)
{
    playSound(210);
}


void playGameAction(void)
{
    playSound(600);
}

void playMenuSelectSound(void)
{
    playSound(1250);
}

void playMenuBackSound(void)
{
    playSound(1000);
}

void playMenuAcknowlege(void)
{
    playSound(900);
}


void processSound()
{
    if (selecting_music)
    {
        return;
    }

    musicTimer();
}
