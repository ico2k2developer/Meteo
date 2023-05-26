//
// Created by ico2k2 on 21/5/2023.
//

#ifndef METEO_HOMEASSISTANT_HPP
#define METEO_HOMEASSISTANT_HPP

#include <mqtt.hpp>

#define HOMEASSISTANT_ADDR  "homeassistant.local"
#define HOMEASSISTANT_PORT  MQTT_PORT_TCP
#define HOMEASSISTANT_QOS   0
#define HOMEASSISTANT_RET   true

#define HOMEASSISTANT_TOPIC         "meteo"
#define HOMEASSISTANT_TOPIC_TEMP    HOMEASSISTANT_TOPIC "/temperature"
#define HOMEASSISTANT_TOPIC_PRESS   HOMEASSISTANT_TOPIC "/pressure"
#define HOMEASSISTANT_TOPIC_HUM     HOMEASSISTANT_TOPIC "/humidity"

uint8_t homeassistant_update(float temp,float press,float hum);

#endif //METEO_HOMEASSISTANT_HPP
