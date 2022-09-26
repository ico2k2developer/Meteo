//
// Created by ico2k2 on 19/07/2022.
//

#ifndef METEO_ADAFRUIT_H
#define METEO_ADAFRUIT_H

#define AIO_SERVER      "io.adafruit.com"
#define AIO_PORT        1883
#define AIO_FINGERPRINT "59 3C 48 0A B1 8B 39 4E 0D 58 50 47 9A 13 55 60 CC A0 1D AF"

#define REFRESH_FEED        10000
#define BASE_FEED           AIO_USERNAME "/feeds/meteo."

#define FEED_RSSI                   BASE_FEED "rssi"
#define FEED_TEMPERATURE            BASE_FEED "temperature"
#define FEED_PRESSURE               BASE_FEED "pressure"
#define FEED_HUMIDITY               BASE_FEED "humidity"
#define FEED_LOGS                   BASE_FEED "log"

#endif //METEO_ADAFRUIT_H
