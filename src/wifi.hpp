//
// Created by ico2k2 on 21/5/2023.
//

#ifndef METEO_WIFI_HPP
#define METEO_WIFI_HPP

#include <config.hpp>
#include <ESP8266WiFi.h>
#include <sntp.h>
#include <passwords.hpp>
#include <timer.hpp>

#define WIFI_INTERVAL_RETRY     5000

#define WIFI_SIZE_PASSWORD      64

const char MEM_DEFAULT_SSID[] PROGMEM =     DEFAULT_SSID;
const char MEM_WIFI01_SSID[] PROGMEM =      WIFI01_SSID;
const char MEM_WIFI02_SSID[] PROGMEM =      WIFI02_SSID;

const char MEM_DEFAULT_PASSWORD[] PROGMEM = DEFAULT_PASSWORD;
const char MEM_WIFI01_PASSWORD[] PROGMEM =  WIFI01_PASSWORD;
const char MEM_WIFI02_PASSWORD[] PROGMEM =  WIFI02_PASSWORD;

const char* const WIFI_SSIDS[] PROGMEM =        {MEM_DEFAULT_SSID,MEM_WIFI01_SSID,MEM_WIFI02_SSID};
const char* const WIFI_PASSWORDS[] PROGMEM =    {MEM_DEFAULT_PASSWORD,MEM_WIFI01_PASSWORD,MEM_WIFI02_PASSWORD};

#define MEM_WIFI_COUNT  3

uint8_t wifi_init(timer_id_t timer_id);

#endif //METEO_WIFI_HPP
