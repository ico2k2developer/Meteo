//
// Created by ico2k2 on 19/07/2022.
//

#ifndef METEO_TYPES_H
#define METEO_TYPES_H

#include <cstdint>

#define BASE_YEAR   2020

typedef struct
{
    uint8_t day,month,year_encoded;
}date_t;

typedef struct
{
    uint16_t time;
    float value;
}sample_t;

typedef date_t* date_p;
typedef sample_t* sample_p;

uint16_t    date_get_year(date_p date);
uint8_t     date_get_month(date_p date);
uint8_t     date_get_day(date_p date);
date_p      date_set_year(date_p date,uint16_t year);
date_p      date_set_month(date_p date,uint8_t month);
date_p      date_set_day(date_p date,uint8_t day);
date_p      date_set_date(date_p date,uint8_t day,uint8_t month,uint16_t year);

uint8_t     sample_get_hour(sample_t sample);
uint8_t     sample_get_min(sample_t sample);
sample_p    sample_set_time(sample_p sample,uint8_t hour,uint8_t min);

float       sample_get_value(sample_t sample);
sample_p    sample_set_value(sample_p sample,float value);
sample_p    sample_copy(sample_p target,sample_t source);


#endif //METEO_TYPES_H
