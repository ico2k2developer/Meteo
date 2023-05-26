//
// Created by Ico on 23/5/2023.
//

#include <homeassistant.hpp>

WiFiClient client;
//TODO: home assistant user & password
Adafruit_MQTT_Client mqtt(&client,HOMEASSISTANT_ADDR,HOMEASSISTANT_PORT,HA_USER,HA_PASSWORD);

uint8_t homeassistant_update(float temp,float press,float hum)
{
    if(mqtt.connected())
    {
        char tmp[10];
        snprintf(tmp,sizeof(tmp) / sizeof(tmp[0]),"%f",temp);
        if(!mqtt.publish(HOMEASSISTANT_TOPIC_TEMP,tmp,HOMEASSISTANT_QOS,HOMEASSISTANT_RET))
            return -2;
        snprintf(tmp,sizeof(tmp) / sizeof(tmp[0]),"%f",press);
        if(!mqtt.publish(HOMEASSISTANT_TOPIC_PRESS,tmp,HOMEASSISTANT_QOS,HOMEASSISTANT_RET))
            return -2;
        snprintf(tmp,sizeof(tmp) / sizeof(tmp[0]),"%f",hum);
        if(!mqtt.publish(HOMEASSISTANT_TOPIC_HUM,tmp,HOMEASSISTANT_QOS,HOMEASSISTANT_RET))
            return -2;
        return 0;
    }
    else
    {
        mqtt.connect();
        return -1;
    }
}