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

#define DIV_TEMP    100
#define DIV_PRESS   10
#define DIV_HUM     100

typedef struct
{
    uint16_t time,value;
}sample_t;

typedef sample_t temp_t;
typedef sample_t press_t;
typedef sample_t hum_t;

typedef date_t* date_p;
typedef sample_t* sample_p;
typedef temp_t* temp_p;
typedef press_t* press_p;
typedef hum_t* hum_p;

uint16_t    date_get_year(date_p date);
uint8_t     date_get_month(date_p date);
uint8_t     date_get_day(date_p date);
date_p      date_set_year(date_p date,uint16_t year);
date_p      date_set_month(date_p date,uint8_t month);
date_p      date_set_day(date_p date,uint8_t day);
date_p      date_set_date(date_p date,uint8_t day,uint8_t month,uint16_t year);

uint8_t     sample_get_hour(sample_p sample);
uint8_t     sample_get_min(sample_p sample);
sample_p    sample_set_time(sample_p sample,uint8_t hour,uint8_t min);
#define     temp_get_hour   sample_get_hour
#define     press_get_hour  sample_get_hour
#define     hum_get_hour    sample_get_hour
#define     temp_get_min    sample_get_min
#define     press_get_min   sample_get_min
#define     hum_get_min     sample_get_min
#define     temp_set_time   sample_set_time
#define     press_set_time  sample_set_time
#define     hum_set_time    sample_set_time

float       temp_get_value(temp_p temp);
float       press_get_value(press_p press);
float       hum_get_value(hum_p hum);
temp_p      temp_set_value(temp_p temp,float value);
press_p     press_set_value(press_p press,float value);
hum_p       hum_set_value(hum_p hum,float value);



#endif //METEO_TYPES_H
