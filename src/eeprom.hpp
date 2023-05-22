//
// Created by ico2k2 on 23/9/2022.
//

#ifndef METEO_EEPROM_HPP
#define METEO_EEPROM_HPP

#include <cstdint>
#include <cstdlib>
#include <Wire.h>
#include <uRTCLib.h>

#define EEPROM_I2C  0x57

#define EEPROM_DELAY    5

#define EEPROM_BUFFER_READ  BUFFER_LENGTH
#define EEPROM_BUFFER_WRITE (EEPROM_BUFFER_READ - 2)

#define EEPROM_PAGE         32

void*       eeprom_read(uint16_t mem_address, void* buffer, uint16_t size);
void        eeprom_write(uint16_t mem_address, void* buffer, uint16_t size);

#endif //METEO_EEPROM_HPP
