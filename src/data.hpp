//
// Created by ico2k2 on 29/9/2022.
//

#ifndef METEO_DATA_HPP
#define METEO_DATA_HPP

#include <eeprom.hpp>

typedef struct
{
    uint8_t hour,minute;
}timeday_t;

typedef timeday_t* timeday_p;

typedef struct
{
    timeday_t time;
    uint32_t value;
}data_t;

typedef data_t* data_p;

typedef struct
{
    uint16_t day;
    data_t temp_min,temp_max;
    data_t press_min,press_max;
    data_t hum_min,hum_max;
}parcel_t;

typedef parcel_t* parcel_p;

#define PARCEL_YEAR     8
#define PARCEL_DAY      16
#define PARCEL_TIME     17
#define PARCEL_TEMP     14
#define PARCEL_PRESS    17
#define PARCEL_HUM      14
#define PARCEL_LAST     PARCEL_DAY
#define PARCEL_POS      16

#define PARCEL_START        (PARCEL_YEAR + PARCEL_LAST + PARCEL_POS)
#define PARCEL_START_POS    (PARCEL_YEAR + PARCEL_LAST)

#define PARCEL_FIELD_TEMP   (2 * (PARCEL_TIME + PARCEL_TEMP))
#define PARCEL_FIELD_PRESS  (2 * (PARCEL_TIME + PARCEL_PRESS))
#define PARCEL_FIELD_HUM    (2 * (PARCEL_TIME + PARCEL_HUM))

#define PARCEL_SIZE     (PARCEL_DAY + PARCEL_FIELD_TEMP + PARCEL_FIELD_PRESS + PARCEL_FIELD_HUM)

#define PARCEL_YEAR_OFF 2022

typedef union
{
    uint16_t data;
    uint8_t bytes[sizeof(data)];
}data16_t;

typedef union
{
    uint32_t data;
    uint8_t bytes[sizeof(data)];
}data32_t;

typedef union
{
    uint16_t value;
    uint8_t data[PARCEL_POS >> 3];
}data_pos_t;

void initData(uint16_t year)
{
    uint8_t data[PARCEL_START >> 3];
    data16_t day;
    data16_t pos;
    day.data = 0;
    pos.data = PARCEL_START;
    data[0] = (year - PARCEL_YEAR_OFF);
    data[1] = day.bytes[0];
    data[2] = day.bytes[1];
    data[3] = pos.bytes[0];
    data[4] = pos.bytes[1];


    eeprom_write(0,data,sizeof(data));

}

data_p loadData(data_p target,uint16_t day)
{
    if(target != nullptr)
    {
        uint8_t data[PARCEL_SIZE >> 3];
        data_pos_t pos;
        eeprom_read(PARCEL_START_POS,pos.data,sizeof(pos.data));


    }
    return target;
}



#endif //METEO_DATA_HPP
