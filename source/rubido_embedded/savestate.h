#ifndef SAVEDSTATE_H
#define SAVEDSTATE_H

#include <stdint.h>

#define SAVE_MAGIC 0xDCDC

//the ESPboy version saved this at the start of its EEPROM, it is kept there so its best pegs
//left are still read
#pragma pack(push, 1)
struct SaveData {
    uint16_t magic;  // always first
    int32_t BestPegsLeft[4]; // array that holds the best amount of pegs left for each difficulty
    uint8_t crc;
};
#pragma pack(pop)

void loadSavedState(void);
void saveSavedState(void);

#endif
