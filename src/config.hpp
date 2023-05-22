//
// Created by Ico on 20/7/2022.
//

#ifndef METEO_CONFIG_HPP
#define METEO_CONFIG_HPP

#include <uRTCLib.h>

#define NTP_SERVER_1            "pool.ntp.org"
#define NTP_SERVER_2            "time.windows.com"
#define NTP_SERVER_3            "time.nist.gov"

#define TZ                      "CET-1CEST,M3.5.0/2,M10.5.0/3"

uint8_t i2c_bus_clear();

#endif //METEO_CONFIG_HPP
