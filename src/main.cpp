#include <main.h>

Adafruit_BME280 bme;
uRTCLib rtc;
WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client,AIO_SERVER,AIO_PORT,AIO_USERNAME,AIO_USERNAME,AIO_KEY);
Adafruit_MQTT_Publish rssi(&mqtt,FEED_RSSI );
Adafruit_MQTT_Publish temperature(&mqtt, FEED_TEMPERATURE);
Adafruit_MQTT_Publish pressure(&mqtt, FEED_PRESSURE);
Adafruit_MQTT_Publish humidity(&mqtt, FEED_HUMIDITY);
Adafruit_MQTT_Publish logs(&mqtt, FEED_LOGS);

void setup()
{
    Serial.begin(115200);
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
    /*ArduinoOTA.onStart([]()
    {
        char str[] = "Incoming OTA update, current version is " VERSION;
        Serial.println(str);
        if(mqtt.connected())
            logs.publish(str);
    });
    ArduinoOTA.onEnd([]()
    {
        char str[] = "OTA update completed";
        Serial.println(str);
    });
    ArduinoOTA.onError([](ota_error_t error)
    {
        char str[100];
        strcpy(str,"OTA update failed: ");
        switch (error)
        {
            case OTA_AUTH_ERROR:
            {
                strcat(str,"auth error");
                break;
            }
            case OTA_BEGIN_ERROR:
            {
                strcat(str,"couldn\'t start download");
                break;
            }
            case OTA_CONNECT_ERROR:
            {
                strcat(str,"connection failed");
                break;
            }
            case OTA_RECEIVE_ERROR:
            {
                strcat(str,"error during download");
                break;
            }
            case OTA_END_ERROR:
            {
                strcat(str,"couldn\'t complete update");
                break;
            }
            default:
            {
                strcat(str,"unknown error");
            }
        }
        Serial.println(str);
        if(mqtt.connected())
            logs.publish(str);
    });
    ArduinoOTA.begin();*/
    NTP.onNTPSyncEvent([](NTPEvent_t event)
    {
        Serial.printf("NTP sync event received: %d\n",event.event);
        if(event.event == timeSyncd)
        {
            Serial.println("NTP successful sync");
            time_t t = NTP.getLastNTPSync();
            Serial.println("a");
            struct tm* time = gmtime(&t);
            Serial.println("b");
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
            Serial.printf("Time synced to %02d:%02d:%02d %d/%d/%d (%d doW)\n",
                          time->tm_hour,
                          time->tm_min,
                          time->tm_sec,
                          time->tm_mday,
                          time->tm_mon,
                          time->tm_year,
                          time->tm_wday
            );
            rtc.set(1,1,1,1,1,15,1);
            Serial.println("c");
            rtc.set(
                    time->tm_sec,
                    time->tm_min,
                    time->tm_hour,
                    dayOfWeek,
                    time->tm_mday,
                    time->tm_mon,
                    time->tm_year - 100);
            Serial.printf("RTC synced to %02hhu:%02hhu:%02hhu %hhu/%hhu/%d (%hhu doW)\n",
                          rtc.hour(),
                          rtc.minute(),
                          rtc.second(),
                          rtc.day(),
                          rtc.month(),
                          rtc.year(),
                          rtc.dayOfWeek());
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

    WiFi.mode(WIFI_STA);

    Serial.println("Setup completed");
    delay(1000);
}

unsigned char connected = 1;

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
            mqtt.connect();
            connected = 1;
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
}