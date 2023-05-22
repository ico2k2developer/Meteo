#include <main.hpp>

Adafruit_BME280 bme;
uRTCLib rtc;
/*WiFiClient client;
Adafruit_MQTT_Client mqtt(&client,AIO_ADDR,AIO_PORT,PROJECT,AIO_USERNAME,AIO_KEY);
Adafruit_MQTT_Publish rssi(&mqtt,FEED_RSSI );
Adafruit_MQTT_Publish temperature(&mqtt, FEED_TEMPERATURE);
Adafruit_MQTT_Publish pressure(&mqtt, FEED_PRESSURE);
Adafruit_MQTT_Publish humidity(&mqtt, FEED_HUMIDITY);
Adafruit_MQTT_Publish logs(&mqtt, FEED_LOGS);

sample_t temp_max,temp_min,press_max,press_min,hum_max,hum_min;
uint64_t serial;

bool_t minMaxUpdates;
#define UPDATE_TEMP_MAX     0
#define UPDATE_TEMP_MIN     1
#define UPDATE_PRESS_MAX    2
#define UPDATE_PRESS_MIN    3
#define UPDATE_HUM_MAX      4
#define UPDATE_HUM_MIN      5*/

void setup()
{
    Serial.begin(115200);
    Serial.println("\n\nSerial port initialized.");
    Serial.printf("Restart reason: %s\n",EspClass::getResetReason().c_str());


    if(i2c_bus_clear())
    {
        Serial.println("Restarting because IC2 bus is not available...");
        EspClass::restart();
    }
    Wire.begin(0,2);

    if(bme.begin())
        Serial.println("BME280 sensor correctly configured");
    else
    {
        Serial.println("Restarting because BME280 is not available...");
        delay(5000);
        EspClass::restart();
    }

    Serial.println("Initiaziling setup");
    rtc.set_model(URTCLIB_MODEL_DS3231);
    rtc.refresh();
    Serial.printf("RTC time is %02i:%02i:%02i %i/%i/%i\n",
                  rtc.hour(),
                  rtc.minute(),
                  rtc.second(),
                  rtc.day(),
                  rtc.month(),
                  rtc.year());
    //client.setFingerprint(AIO_FINGERPRINT);

    /*rtc.refresh();

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

    serial = millis();*/

    timer_init(8);

    WiFi.mode(WIFI_STA);
    //WiFi.begin(DEFAULT_SSID,DEFAULT_PASSWORD);

    Serial.println("Setup completed\n");
}


typedef enum
{
    NOT_SCANNING,
    SCANNING,
    SCAN_ENDED,
}scan_t;

unsigned char firstConnection = 1;
unsigned char connected = 0;
scan_t scanner = NOT_SCANNING;
unsigned long ms;
uint8_t wifi_selected,wifi_count;
uint8_t wifi_found[MEM_WIFI_COUNT];
/*unsigned int update = 0;
int8_t mqttError;*/

void loop()
{
    micros64();



    /*switch(WiFi.status())
    {
        case WL_CONNECTED:
        {
            if(!connected)
            {
                connected = 1;
                network = 0;
                Serial.printf("Connected to %s\n",WiFi.SSID().c_str());
                if(firstConnection)
                {
                    firstConnection = 0;
                    configTime(NTP_TIMEZONE * 3600,DST_OFFSET * 3600,NTP_SERVER_1,NTP_SERVER_2,NTP_SERVER_3);
                }
                else
                    sntp_init();
                time_t data;
                time(&data);
                struct tm * info = localtime(&data);
                char tmp[100];
                strftime(tmp,100,"%A, %d %B &G, %H:%M:%S\n",info);
                Serial.print(tmp);
            }
            break;
        }
        case WL_NO_SSID_AVAIL:
        case WL_CONNECT_FAILED:
        case WL_WRONG_PASSWORD:
        {
            Serial.printf("Could not connect to " WIFI_SSID_FORMAT "\n",network);
            network++;
        }
        default:
        {
            connected = 0;
            if(!firstConnection)
                sntp_stop();
            char ssid[WL_SSID_MAX_LENGTH];
            snprintf(ssid,WL_SSID_MAX_LENGTH,WIFI_SSID_FORMAT,network);
            WiFi.begin("WIFI-00",WIFI_PASSWORD);
            Serial.printf("Trying to connect to %s\n","WIFI-00");
        }
    }*/
}


/*void loop()
{
    if(WiFi.isConnected())
    {
        //ArduinoOTA.handle();
        if(!connected)
        {
            Serial.println("Connected");
            ntp.begin();
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
                    pressure.publish(bme.readPressure() / 100.0f,4);
                    humidity.publish(bme.readHumidity());

                    char tmp[100];
                    if(bool_getValue(minMaxUpdates,UPDATE_TEMP_MAX))
                    {
                        sprintf(tmp,"New highest temperature of the day reached: %.2fC",sample_get_value(temp_max));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_TEMP_MIN))
                    {
                        sprintf(tmp,"New lowest temperature of the day reached: %.2fC",sample_get_value(temp_min));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_PRESS_MAX))
                    {
                        sprintf(tmp,"New highest absolute pressure of the day reached: %.2fhPa",sample_get_value(press_max));
                        Serial.println(tmp);
                        logs.publish(tmp);
                    }
                    if(bool_getValue(minMaxUpdates,UPDATE_PRESS_MIN))
                    {
                        sprintf(tmp,"New lowest absolute pressure of the day reached: %.2fhPa",sample_get_value(press_min));
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

        if(ntp.update())
        {
            Serial.println("NTP update received");
            syncTime(ntp,rtc);
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
    float press = bme.readPressure();
    float hum = bme.readHumidity();

    if((millis() - serial) > 500)
    {
        Serial.printf("%.2fC, %.2fPa, %.2f%% humidity\n",temp,press,hum);
        serial = millis();
    }


    if(temp > sample_get_value(temp_max))
    {
        rtc.refresh();
        sample_set_time(&temp_max,rtc.hour(),rtc.minute());
        sample_set_value(&temp_max,temp);
        bool_setValue(&minMaxUpdates,UPDATE_TEMP_MAX,1);
    }
    else if(temp < sample_get_value(temp_min))
    {
        rtc.refresh();
        sample_set_time(&temp_min,rtc.hour(),rtc.minute());
        sample_set_value(&temp_min,temp);
        bool_setValue(&minMaxUpdates,UPDATE_TEMP_MIN,1);
    }

    press /= 100.0f;

    if(press > sample_get_value(press_max))
    {
        rtc.refresh();
        sample_set_time(&press_max,rtc.hour(),rtc.minute());
        sample_set_value(&press_max,press);
        bool_setValue(&minMaxUpdates,UPDATE_PRESS_MAX,1);
    }
    else if(press < sample_get_value(press_min))
    {
        rtc.refresh();
        sample_set_time(&press_min,rtc.hour(),rtc.minute());
        sample_set_value(&press_min,press);
        bool_setValue(&minMaxUpdates,UPDATE_PRESS_MIN,1);
    }

    if(hum > sample_get_value(hum_max))
    {
        rtc.refresh();
        sample_set_time(&hum_max,rtc.hour(),rtc.minute());
        sample_set_value(&hum_max,hum);
        bool_setValue(&minMaxUpdates,UPDATE_HUM_MAX,1);
    }
    else if(hum < sample_get_value(hum_min))
    {
        rtc.refresh();
        sample_set_time(&hum_min,rtc.hour(),rtc.minute());
        sample_set_value(&hum_min,hum);
        bool_setValue(&minMaxUpdates,UPDATE_HUM_MIN,1);
    }

}*/