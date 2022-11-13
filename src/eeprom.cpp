//
// Created by ico2k2 on 23/9/2022.
//

#include "eeprom.h"

void* eeprom_read(uint16_t mem_address, void* buffer, uint16_t size)
{
    if(size > 0 && buffer != nullptr)
    {
        uint16_t page,end;
        uint16_t i = 0;
        while(i < size)
        {
            page = mem_address / EEPROM_PAGE;
            end = EEPROM_PAGE * (page + 1) - mem_address;
#if EEPROM_PAGE > EEPROM_BUFFER_READ
            if(end > EEPROM_BUFFER_READ)
                end = EEPROM_BUFFER_READ;
#endif
            if(end > (size - i))
                end = size - i;
            Wire.beginTransmission(EEPROM_I2C);
            Wire.write((int)(mem_address >> 8));
            Wire.write((int)(mem_address & 0xFF));
            Wire.endTransmission();
            delay(EEPROM_DELAY);
            Wire.requestFrom(EEPROM_I2C,end);
            delay(EEPROM_DELAY);
            end += mem_address;
            for(; mem_address < end; mem_address++)
            {
                ((uint8_t*)buffer)[i++] = Wire.read();
            }
        }
    }
    return buffer;
}

void eeprom_write(uint16_t mem_address, void* buffer, uint16_t size) {
    if (size == 0 || buffer == nullptr)
        return;
    uint16_t page, end;
    uint16_t i = 0;
    while (i < size) {
        page = mem_address / EEPROM_PAGE;
        end = EEPROM_PAGE * (page + 1);
#if EEPROM_PAGE > EEPROM_BUFFER_WRITE
        if((end - mem_address) > EEPROM_BUFFER_WRITE)
            end = mem_address + EEPROM_BUFFER_WRITE;
#endif
        if ((end - mem_address) > (size - i))
            end = mem_address + size - i;
        Wire.beginTransmission(EEPROM_I2C);
        Wire.write((int) (mem_address >> 8));
        Wire.write((int) (mem_address & 0xFF));
        delay(EEPROM_DELAY);
        for (; mem_address < end; mem_address++) {
            Wire.write(((uint8_t *) buffer)[i++]);
        }
        Wire.endTransmission();
    }
}