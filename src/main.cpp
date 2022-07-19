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
    Wire.begin(0,2);
    rtc.refresh();
    client.setFingerprint(AIO_FINGERPRINT);
    ledBuiltinSetup();
    ArduinoOTA.onStart([]()
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
    ArduinoOTA.begin();

    if(bme.begin())
        Serial.println("BME280 sensor correctly configured");
    else
    {
        Serial.println("BME280 not found, restarting...");
        EspClass::restart();
    }

    WiFi.mode(WIFI_STA);

    Serial.println("Setup completed");
    delay(1000);
}

void loop()
{
    rtc.refresh();
    if(WiFi.isConnected())
    {
        if(rtc.hour() == 0)
        {

        }
    }
    else
    {
        WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    }
}