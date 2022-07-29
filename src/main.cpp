#include <main.h>

Adafruit_BME280 bme;
uRTCLib rtc;
WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client,AIO_SERVER,AIO_PORT,PROJECT,AIO_USERNAME,AIO_KEY);
Adafruit_MQTT_Publish rssi(&mqtt,FEED_RSSI );
Adafruit_MQTT_Publish temperature(&mqtt, FEED_TEMPERATURE);
Adafruit_MQTT_Publish pressure(&mqtt, FEED_PRESSURE);
Adafruit_MQTT_Publish humidity(&mqtt, FEED_HUMIDITY);
Adafruit_MQTT_Publish logs(&mqtt, FEED_LOGS);

uint8_t ntpEvent = 0;

sample_t temp_max,temp_min,press_max,press_min,hum_max,hum_min;

bool_t minMaxUpdates;
#define UPDATE_TEMP_MAX     0
#define UPDATE_TEMP_MIN     1
#define UPDATE_PRESS_MAX    2
#define UPDATE_PRESS_MIN    3
#define UPDATE_HUM_MAX      4
#define UPDATE_HUM_MIN      5

void setup()
{
    Serial.begin(74880);
    Serial.println("\nInitiaziling setup");
    Wire.begin(0,2);
    rtc.set_model(URTCLIB_MODEL_DS3231);
    rtc.refresh();
    Serial.printf("Current time is %02i:%02i:%02i %i/%i/%i\n",
                  rtc.hour(),
                  rtc.minute(),
                  rtc.second(),
                  rtc.day(),
                  rtc.month(),
                  rtc.year());
    client.setFingerprint(AIO_FINGERPRINT);
    NTP.setTimeZone(TZ_Etc_UTC);
    NTP.setInterval(NTP_UPDATE_INTERVAL,NTP_UPDATE_INTERVAL_L);
    NTP.onNTPSyncEvent([](NTPEvent_t event)
    {
        Serial.printf("NTP sync event received: %d\n",event.event);
        if(event.event == timeSyncd)
        {
            ntpEvent = 1;
        }
    });
    NTP.begin(DEFAULT_NTP_SERVER,false);

    if(bme.begin())
        Serial.println("BME280 sensor correctly configured");
    else
    {
        Serial.println("BME280 not found, restarting...");
        delay(5000);
        EspClass::restart();
    }

    rtc.refresh();

    sample_set_value(&temp_max,bme.readTemperature());
    sample_set_time(&temp_max,rtc.hour(),rtc.minute());
    sample_copy(&temp_min,temp_max);

    sample_set_value(&press_max,bme.readPressure() / 100.0f);
    sample_set_time(&press_max,rtc.hour(),rtc.minute());
    sample_copy(&press_min,press_max);

    sample_set_value(&hum_max,bme.readHumidity());
    sample_set_time(&hum_max,rtc.hour(),rtc.minute());
    sample_copy(&hum_min,hum_max);

    minMaxUpdates = 0;

    WiFi.mode(WIFI_STA);

    Serial.println("Setup completed");
    delay(1000);
}

unsigned char connected = 1;
unsigned int update = 0;
int8_t mqttError;

void loop()
{
    rtc.refresh();
    if(WiFi.isConnected())
    {
        //ArduinoOTA.handle();
        if(!connected)
        {
            Serial.println("Connected");
            NTP.getTime();
            mqttError = mqtt.connect();
            connected = 1;
        }
        else
        {
            if(mqtt.connected())
            {
                if(connected == 1 || ((millis() - update) > MQTT_UPDATE_INTERVAL))
                {
                    update = millis();
                    rssi.publish(WiFi.RSSI());
                    temperature.publish(bme.readTemperature());
                    pressure.publish(bme.readPressure() / 100.0f);
                    humidity.publish(bme.readHumidity());

                    char tmp[100];
                    if(bool_getValue(minMaxUpdates,UPDATE_TEMP_MAX))
                    {
                        sprintf(tmp,"New highest temperature of the day reached: %.2f°C",sample_get_value(temp_max));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_TEMP_MIN))
                    {
                        sprintf(tmp,"New lowest temperature of the day reached: %.2f°C",sample_get_value(temp_min));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_PRESS_MAX))
                    {
                        sprintf(tmp,"New highest absolute pressure of the day reached: %.2f hPa",sample_get_value(press_max));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_PRESS_MIN))
                    {
                        sprintf(tmp,"New lowest absolute pressure of the day reached: %.2f hPa",sample_get_value(press_min));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_HUM_MAX))
                    {
                        sprintf(tmp,"New highest relative humidity of the day reached: %.2f%%",sample_get_value(hum_max));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_HUM_MIN))
                    {
                        sprintf(tmp,"New lowest relative humidity of the day reached: %.2f%%",sample_get_value(hum_min));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    minMaxUpdates = 0;
                    Serial.println("Updated MQTT data");
                }
                if(connected == 1)
                {
                    char tmp[100];
                    sprintf(tmp,"Connection established through network %s, now sending updates every %.1fs",
                            WiFi.SSID().c_str(),MQTT_UPDATE_INTERVAL / 1000.0);
                    Serial.println(tmp);
                    logs.publish(tmp);
                    connected = 2;
                }
            }
            else
            {
                connected = 1;
                Serial.print("MQTT error: ");
                Serial.print(mqtt.connectErrorString(mqttError));
                Serial.println(", trying to connect...");
                mqttError = mqtt.connect();
            }
        }

        if(ntpEvent)
        {
            ntpEvent = 0;

            Serial.println("NTP successful sync");
            time_t t = NTP.getLastNTPSync();
            struct tm* time = gmtime(&t);
            uint8_t dayOfWeek;
            switch(time->tm_wday)
            {
                case 0:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_SUNDAY;
                    break;
                }
                case 1:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_MONDAY;
                    break;
                }
                case 2:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_TUESDAY;
                    break;
                }
                case 3:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_WEDNESDAY;
                    break;
                }
                case 4:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_THURSDAY;
                    break;
                }
                case 5:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_FRIDAY;
                    break;
                }
                case 6:
                {
                    dayOfWeek = URTCLIB_WEEKDAY_SATURDAY;
                    break;
                }
                default:
                {
                    dayOfWeek = 0;
                }
            }
            rtc.set(
                    time->tm_sec,
                    time->tm_min,
                    time->tm_hour,
                    dayOfWeek,
                    time->tm_mday,
                    time->tm_mon,
                    time->tm_year - 100);
            rtc.refresh();
            {
                char tmp[100];
                sprintf(tmp,"RTC synced to UTC time %02hhu:%02hhu:%02hhu %hhu/%hhu/%d (%hhu doW)",
                        rtc.hour(),
                        rtc.minute(),
                        rtc.second(),
                        rtc.day(),
                        rtc.month(),
                        ((uint16_t)rtc.year()) + 2000,
                        rtc.dayOfWeek());
                Serial.println(tmp);
                logs.publish(tmp);
            }

        }
    }
    else
    {
        if(connected)
        {
            Serial.println("Not connected, trying to connect...");
            WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
            connected = 0;
        }
    }

    float temp = bme.readTemperature();
    float press = bme.readPressure() / 100.0f;
    float hum = bme.readHumidity();
    rtc.refresh();

    if(temp > sample_get_value(temp_max))
    {
        sample_set_time(&temp_max,rtc.hour(),rtc.minute());
        sample_set_value(&temp_max,temp);
        bool_setValue(&minMaxUpdates,UPDATE_TEMP_MAX,1);
    }
    else if(temp < sample_get_value(temp_min))
    {
        sample_set_time(&temp_min,rtc.hour(),rtc.minute());
        sample_set_value(&temp_min,temp);
        bool_setValue(&minMaxUpdates,UPDATE_TEMP_MIN,1);
    }

    if(press > sample_get_value(press_max))
    {
        sample_set_time(&press_max,rtc.hour(),rtc.minute());
        sample_set_value(&press_max,press);
        bool_setValue(&minMaxUpdates,UPDATE_PRESS_MAX,1);
    }
    else if(press < sample_get_value(press_min))
    {
        sample_set_time(&press_min,rtc.hour(),rtc.minute());
        sample_set_value(&press_min,press);
        bool_setValue(&minMaxUpdates,UPDATE_PRESS_MIN,1);
    }

    if(hum > sample_get_value(hum_max))
    {
        sample_set_time(&hum_max,rtc.hour(),rtc.minute());
        sample_set_value(&hum_max,hum);
        bool_setValue(&minMaxUpdates,UPDATE_HUM_MAX,1);
    }
    else if(hum < sample_get_value(hum_min))
    {
        sample_set_time(&hum_min,rtc.hour(),rtc.minute());
        sample_set_value(&hum_min,hum);
        bool_setValue(&minMaxUpdates,UPDATE_HUM_MIN,1);
    }

}