//
// Created by Ico on 23/5/2023.
//

#include <homeassistant.hpp>

WiFiClient client;
//TODO: home assistant user & password
Adafruit_MQTT_Client mqtt(&client,HOMEASSISTANT_ADDR,HOMEASSISTANT_PORT,nullptr,nullptr);

void homeassistant_connect()
{
    mqtt.connect();
}

void homeassistant_update(float temp,float press,float hum)
{
    char tmp[10];
    snprintf(tmp,sizeof(tmp) / sizeof(tmp[0]),"%f",temp);
    mqtt.publish(HOMEASSISTANT_TOPIC_TEMP,tmp,HOMEASSISTANT_QOS,HOMEASSISTANT_RET);
    snprintf(tmp,sizeof(tmp) / sizeof(tmp[0]),"%f",press);
    mqtt.publish(HOMEASSISTANT_TOPIC_PRESS,tmp,HOMEASSISTANT_QOS,HOMEASSISTANT_RET);
    snprintf(tmp,sizeof(tmp) / sizeof(tmp[0]),"%f",hum);
    mqtt.publish(HOMEASSISTANT_TOPIC_HUM,tmp,HOMEASSISTANT_QOS,HOMEASSISTANT_RET);
}