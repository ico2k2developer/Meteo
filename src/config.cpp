//
// Created by ico2k2 on 25/9/2022.
//

#include "config.h"

void syncTime(NTPClient client,uRTCLib rtc)
{
    unsigned long epoch = client.getEpochTime() + NTP_TIMEZONE * 60 * 60;
    uint16_t quartet = epoch / (60 * 60 * 24 * (365 * 4 + 1));
    uint16_t day = (epoch - quartet * (60 * 60 * 24 * (365 * 4 + 1))) / (60 * 60 * 24);
    uint16_t year;
    if(day < (365 * 2))
        year = day / 365;
    else
        year = day < (365 * 3 + 1) ? 2 : 3;
    day -= year * 365;
    if(year > 2)
        day--;
    uint8_t mday = 1,month = 0,i;
    uint16_t tmp,last = 0;
    for(i = 0; i < (sizeof(DAY0S) / sizeof(DAY0S[0])); i++)
    {
        tmp = DAY0S[i];
        if(year == 2)
        {
            if(i == 1)
                tmp++;
            else if(i == 2)
                day--;
        }
        if(day < tmp)
        {
            month = i + 1;
            mday += day - last;
            break;
        }
        last = tmp;
    }
    year += quartet * 4 - 30;

    uint8_t wk = client.getDay();
    if(wk == 0)
        wk = 7;

    rtc.set(
            client.getSeconds(),client.getMinutes(),
            client.getHours() + NTP_TIMEZONE,wk,mday,month,year);

    Serial.printf("Current time is %d/%d/%d %d:%d:%d\n",
                  rtc.day(),rtc.month(),rtc.year() + 2000,
                  rtc.hour(),rtc.minute(),rtc.second());




}