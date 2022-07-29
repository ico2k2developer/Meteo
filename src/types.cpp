//
// Created by ico2k2 on 19/07/2022.
//
#include <types.h>

uint16_t date_get_year(date_p date)
{
    return ((uint16_t)(date->year_encoded)) + BASE_YEAR;
}

uint8_t date_get_month(date_p date)
{
    return date->month;
}

uint8_t date_get_day(date_p date)
{
    return date->day;
}

date_p date_set_year(date_p date,uint16_t year)
{
    date->year_encoded = ((uint8_t)(year - BASE_YEAR));
    return date;
}

date_p date_set_month(date_p date,uint8_t month)
{
    date->month = month;
    return date;
}

date_p date_set_day(date_p date,uint8_t day)
{
    date->day = day;
    return date;
}

date_p date_set_date(date_p date,uint8_t day,uint8_t month,uint16_t year)
{
    date_set_day(date,day);
    date_set_month(date,month);
    date_set_year(date,year);
    return date;
}

uint8_t sample_get_hour(sample_t sample)
{
    return sample.time / 60;
}

uint8_t sample_get_min(sample_t sample)
{
    return sample.time - sample_get_hour(sample) * 60;
}

sample_p sample_set_time(sample_p sample,uint8_t hour,uint8_t min)
{
    sample->time = hour * 60 + min;
    return sample;
}

float sample_get_value(sample_t sample)
{
    return sample.value;
}

sample_p sample_set_value(sample_p sample,float value)
{
    sample->value = value;
    return sample;
}

sample_p sample_copy(sample_p target,sample_t source)
{
    target->time = source.time;
    target->value = source.value;
    return target;
}
