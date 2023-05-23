//
// Created by ico2k2 on 19/07/2022.
//

#ifndef METEO_MAIN_HPP
#define METEO_MAIN_HPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_BME280.h>

#include <config.hpp>
#include <homeassistant.hpp>
#include <version.hpp>

PROGMEM

#define LED_ON              LOW
#define LED_OFF             HIGH

#define outSetup(x)         pinMode(x,OUTPUT)
#define ledBuiltinSetup()   outSetup(LED_BUILTIN)
#define out(x,y)            digitalWrite(x,y)
#define ledBuiltin(x)       out(LED_BUILTIN,x)


#define TIMER_COUNT     2

#define TIMER_WIFI          0x1
#define TIMER_CONNECTION    0x2

void wifiScanResult(int foundCount);

#endif //METEO_MAIN_HPP
