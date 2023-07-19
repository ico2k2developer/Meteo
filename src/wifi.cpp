//
// Created by ico2k2 on 21/5/2023.
//

#include <wifi.hpp>

WiFiEventHandler connected_h,disconnected_h;
wifi_connected_t connected_c;
wifi_disconnected_t disconnected_c;
uint16_t count_scan,pos;
timer_id_t timer;

uint8_t check(timer_id_t id,timer_trigger_t trigger);
void scan();
void connection_attempt();
void scan_callback(int count);

void connected_callback(const WiFiEventStationModeConnected &event)
{
    timer_remove(timer);
    Serial.print("Connected to ");
    Serial.print(event.ssid);
    Serial.printf(", channel %hhu\n",event.channel);
    connected_c();
}

void disconnected_callback(const WiFiEventStationModeDisconnected &event)
{
    Serial.print("Disconnected from ");
    Serial.print(event.ssid);
    Serial.printf(" with reason %d\n",event.reason);
    disconnected_c();
    scan();
}

uint8_t check(timer_id_t id,timer_trigger_t trigger)
{
    if(WiFi.isConnected())
        timer_remove(timer);
    else
    {
        connection_attempt();
        timer_set_rel(timer,WIFI_INTERVAL_CHECK,&check);
    }
    return TIMER_RESULT_CONTINUE;
}

void scan()
{
    Serial.println("Attempting to perform a new scan.");
    WiFi.scanNetworksAsync(scan_callback,false);
}

void connection_attempt()
{
    if(pos >= count_scan)
        scan();
    else
    {
        uint16_t i1,i2;
        uint8_t found = 0;
        char name[WL_SSID_MAX_LENGTH + 1];
        char password[WIFI_SIZE_PASSWORD + 1];
        char buffer[MEM_WIFI_COUNT][WL_SSID_MAX_LENGTH + 1];
        for(i1 = 0; i1 < MEM_WIFI_COUNT; i1++)
            strncpy_P(buffer[i1],(char*)pgm_read_dword(&(WIFI_SSIDS[i1])),WL_SSID_MAX_LENGTH + 1);
        for(i1 = pos; i1 < count_scan && !found; i1++)
        {
            strncpy(name,WiFi.SSID(i1).c_str(),WL_SSID_MAX_LENGTH + 1);
            for(i2 = 0; i2 < MEM_WIFI_COUNT; i2++)
            {
                if(buffer[i2] != nullptr)
                {
                    Serial.printf("Comparing \"%s\" & \"%s\": ",name,buffer[i2]);
                    if(strcmp(name,buffer[i2]) == 0)
                    {
                        Serial.println("found!");
                        strncpy_P(password,(char*)pgm_read_dword(&(WIFI_PASSWORDS[i2])),WIFI_SIZE_PASSWORD + 1);
                        Serial.printf("\tname: %s, password: %s\n",name,password);
                        WiFi.begin(name,password);
                        pos = i1 + 1;
                        found = 1;
                        break;
                    }
                    else
                        Serial.println("not found.");
                }
            }
        }
        if(found)
            Serial.println("No need to perform a new scan.");
        else
        {
            delay(2500);
            scan();
        }
    }
}

void scan_callback(int count)
{
    Serial.printf("WiFi scan completed: found %i\n",count);
    pos = 0;
    count_scan = count;
    connection_attempt();
}

void wifi_init(timer_id_t id,wifi_connected_t connected,wifi_disconnected_t disconnected)
{
    connected_c = connected;
    disconnected_c = disconnected;
    connected_h = WiFi.onStationModeConnected(connected_callback);
    disconnected_h = WiFi.onStationModeDisconnected(disconnected_callback);
    WiFi.mode(WIFI_STA);
    count_scan = 0;
    timer = id;
    scan();
}