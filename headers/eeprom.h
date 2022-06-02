#ifndef EEPROM_H
#define EEPROM_H
#include "EEPROM.h"
#include "game.h"

void storeGameBoard()
{
    int address = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            address = address + 1;
            uint8_t high = gameBoard[i][j] >> 8;
            uint8_t low = gameBoard[i][j] & 0xFF;
            EEPROM.write(address++, high);
            EEPROM.write(address, low);
        }
    }
}

void loadGameBoard()
{
    int address = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            address = address + 1;
            uint8_t high; 
            uint8_t low;
            high = EEPROM.read(address++);
            low = EEPROM.read(address);
            uint16_t data = 0x00;
            data = (high << 8) & 0xFF00;
            data = data | low;
            gameBoard[i][j] = data; 
        }
    }
}
#endif