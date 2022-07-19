//
// Created by ico2k2 on 19/07/2022.
//

#ifndef METEO_ADAFRUIT_H
#define METEO_ADAFRUIT_H

#define AIO_SERVER      "io.adafruit.com"
#define AIO_PORT        8883
#define AIO_FINGERPRINT "26 96 1C 2A 51 07 FD 15 80 96 93 AE F7 32 CE B9 0D 01 55 C4"

#define REFRESH_FEED        10000
#define BASE_FEED           AIO_USERNAME "/feeds/meteo."

#define FEED_RSSI                   BASE_FEED "rssi"
#define FEED_TEMPERATURE            BASE_FEED "temperature"
#define FEED_PRESSURE               BASE_FEED "pressure"
#define FEED_HUMIDITY               BASE_FEED "humidity"
#define FEED_LOGS                   BASE_FEED "log"

#endif //METEO_ADAFRUIT_H
