//
// Created by ico2k2 on 19/07/2022.
//

#ifndef METEO_MAIN_H
#define METEO_MAIN_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_BME280.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <uRTCLib.h>

#include <version.h>
#include <passwords.h>
#include <adafruit.h>
#include <types.h>

#define LED_ON              LOW
#define LED_OFF             HIGH

#define outSetup(x)         pinMode(x,OUTPUT)
#define ledBuiltinSetup()   outSetup(LED_BUILTIN)
#define out(x,y)            digitalWrite(x,y)
#define ledBuiltin(x)       out(LED_BUILTIN,x)

#endif //METEO_MAIN_H
