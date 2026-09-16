#include <string.h>
#include <stdint.h>
#include "commonvars.h"
#include "savestate.h"

//everything saved has to fit in what the platform stores
static_assert(sizeof(SaveData) <= PLATFORM_STORAGE_SIZE, "the save state does not fit in PLATFORM_STORAGE_SIZE");

uint8_t calcCRC(void *data, size_t len) {
  uint8_t crc = 0;
  uint8_t *ptr = (uint8_t *)data;
  for (size_t i = 0; i < len; i++) {
    crc ^= ptr[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x07;
      else
        crc <<= 1;
    }
  }
  return crc;
}

void loadSavedState(void)
{
    Platform_StorageRead(0, (uint8_t*)&saveData, sizeof(SaveData));
    //needs to be -uint8t size because of crc not included in calculation
    uint8_t crc = calcCRC(&saveData, sizeof(SaveData) - sizeof(uint8_t));
    if (saveData.magic != SAVE_MAGIC || saveData.crc != crc)
    {
        Platform_Log("save state invalid, loading defaults\n");
        memset(&saveData, 0, sizeof(SaveData));
        saveData.magic = SAVE_MAGIC;
    }
    else
    {
        Platform_Log("save state valid, loaded scores\n");
    }
}

void saveSavedState(void)
{
    saveData.crc = calcCRC(&saveData, sizeof(SaveData) - sizeof(uint8_t));
    Platform_StorageWrite(0, (const uint8_t*)&saveData, sizeof(SaveData));
    Platform_Log("saved, crc: 0x%02X\n", saveData.crc);
}
