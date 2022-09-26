//
// Created by Ico on 20/7/2022.
//

#ifndef METEO_CONFIG_H
#define METEO_CONFIG_H

#include <NTPClient.h>
#include <uRTCLib.h>

#define NTP_SERVER              "pool.ntp.org"
#define NTP_OFFSET              0
//#define NTP_UPDATE_INTERVAL     (3*60*60000)
#define NTP_UPDATE_INTERVAL     60000 * 60
#define NTP_TIMEZONE            (+1)


#define DST_ON_DAY      27
#define DST_ON_MONTH    3
#define DST_ON_HOUR     2

#define DST_OFF_DAY     30
#define DST_OFF_MONTH   10
#define DST_OFF_HOUR    3

#define DST_OFFSET      (60*60000)

#define DAYS_JAN        31
#define DAYS_FEB        28
#define DAYS_MAR        31
#define DAYS_APR        30
#define DAYS_MAY        31
#define DAYS_JUN        30
#define DAYS_JUL        31
#define DAYS_AUG        31
#define DAYS_SEP        30
#define DAYS_OCT        31
#define DAYS_NOV        30
#define DAYS_DEC        31

#define DAY0_JAN        0
#define DAY0_FEB        DAYS_JAN
#define DAY0_MAR        (DAY0_FEB + DAYS_FEB)
#define DAY0_APR        (DAY0_MAR + DAYS_MAR)
#define DAY0_MAY        (DAY0_APR + DAYS_APR)
#define DAY0_JUN        (DAY0_MAY + DAYS_MAY)
#define DAY0_JUL        (DAY0_JUN + DAYS_JUN)
#define DAY0_AUG        (DAY0_JUL + DAYS_JUL)
#define DAY0_SEP        (DAY0_AUG + DAYS_AUG)
#define DAY0_OCT        (DAY0_SEP + DAYS_SEP)
#define DAY0_NOV        (DAY0_OCT + DAYS_OCT)
#define DAY0_DEC        (DAY0_NOV + DAYS_NOV)
#define DAY0_YEAR       (DAY0_DEC + DAYS_DEC)

const uint16_t DAY0S[]    = {
        DAY0_FEB,DAY0_MAR,DAY0_APR,DAY0_MAY,
        DAY0_JUN,DAY0_JUL,DAY0_AUG,DAY0_SEP,
        DAY0_OCT,DAY0_NOV,DAY0_DEC,DAY0_YEAR};

#define MONTH_JAN       1
#define MONTH_FEB       2
#define MONTH_MAR       3
#define MONTH_APR       4
#define MONTH_MAY       5
#define MONTH_JUN       6
#define MONTH_JUL       7
#define MONTH_AUG       8
#define MONTH_SEP       9
#define MONTH_OCT       10
#define MONTH_NOV       11
#define MONTH_DEC       12

#define MQTT_UPDATE_INTERVAL    10000

void syncTime(NTPClient client,uRTCLib rtc);

#endif //METEO_CONFIG_H
