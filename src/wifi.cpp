//
// Created by ico2k2 on 21/5/2023.
//

#include <utility>
#include <wifi.hpp>

timer_id_t id = 0;
uint8_t status = WIFI_NOT_CONNECTED;
uint16_t found = 0;
wifi_connected_t callback_con;
wifi_disconnected_t callback_disc;
uint16_t pos = 0;

uint8_t wifi_status()
{
    id = 0;
    return status;
}

uint8_t wifi_check(timer_id_t id,timer_trigger_t trigger);

void wifi_scan_result(int foundCount)
{
    found = foundCount;
    status = WIFI_SCAN_ENDED;
    timer_trigger(id);
    /*if(foundCount > 0)
    {
        uint32_t i1,i2;
        char name[WL_SSID_MAX_LENGTH];
        char buffer[MEM_WIFI_COUNT][WL_SSID_MAX_LENGTH];
        memset(wifi_found,0,sizeof(uint8_t) * MEM_WIFI_COUNT);
        wifi_count = 0;
        for(i1 = 0; i1 < MEM_WIFI_COUNT; i1++)
        {
            strncpy_P(buffer[i1],(char*) pgm_read_dword(&(WIFI_SSIDS[i1])), WL_SSID_MAX_LENGTH);
        }
        for(i1 = 0; i1 < foundCount; i1++)
        {
            strncpy(name,WiFi.SSID(i1).c_str(),WL_SSID_MAX_LENGTH);
            for(i2 = 0; i2 < MEM_WIFI_COUNT; i2++)
            {
                if(buffer[i2] != nullptr)
                {
                    if(strcmp(name, buffer[i2]) == 0)
                    {
                        buffer[i2] = nullptr;
                        found1[f1] = i2;
                        f1++;
                        break;
                    }
                }
            }
        }
        scanning = false;
        scanFinished = true;
        scanner = SCAN_ENDED;
        for(i1 = 0; i1 < MEM_WIFI_COUNT; i1++)
        {
            free(buffer[i1]);
        }
    }
    else
        scanner = NOT_SCANNING;*/
}

uint8_t wifi_check(timer_id_t timer_id,timer_trigger_t trigger)
{
    timer_set_rel(id,WIFI_INTERVAL_SCANNING * 1000,&wifi_check);
    Serial.printf("WiFi status: %d, ESP WiFi status: %d\n",status,WiFi.status());
    switch(status)
    {
        case WIFI_CONNECTED:
        {
            wl_status_t s = WiFi.status();
            if(s == WL_DISCONNECTED || s == WL_CONNECTION_LOST)
            {
                status = WIFI_NOT_CONNECTED;
                if(callback_con != nullptr)
                    callback_disc(trigger);
                wifi_check(id,trigger);
            }
            break;
        }
        case WIFI_CONNECTING:
        {
            switch(WiFi.status())
            {
                case WL_CONNECTED:
                {
                    status = WIFI_CONNECTED;
                    Serial.printf("Connected to %s\n",WiFi.SSID().c_str());
                    configTime(TZ,NTP_SERVER_1,NTP_SERVER_2,NTP_SERVER_3);
                    timer_set_rel(id,WIFI_INTERVAL_CONNECTED * 1000,nullptr);
                    if(callback_con != nullptr)
                        callback_con(trigger);
                    break;
                }
                case WL_NO_SSID_AVAIL:
                case WL_CONNECT_FAILED:
                case WL_CONNECTION_LOST:
                case WL_WRONG_PASSWORD:
                case WL_DISCONNECTED:
                {
                    status = WIFI_NOT_CONNECTED;
                    wifi_check(id,trigger);
                    break;
                }
            }
            break;
        }
        case WIFI_NOT_CONNECTED:
        {
            WiFi.mode(WIFI_STA);
            WiFi.scanNetworksAsync(&wifi_scan_result,true);
            status = WIFI_SCANNING;
            timer_set_rel(id,WIFI_INTERVAL_SCANNING * 1000,nullptr);
            break;
        }
        case WIFI_SCAN_ENDED:
        {
            status = WIFI_NOT_CONNECTED;
            if(found)
            {
                uint16_t i1,i2 = 0;
                char name[WL_SSID_MAX_LENGTH + 1];
                char buffer[MEM_WIFI_COUNT][WL_SSID_MAX_LENGTH + 1];
                //memset(wifi_found,0,sizeof(uint8_t) * MEM_WIFI_COUNT);
                //wifi_count = 0;
                for(i1 = 0; i1 < MEM_WIFI_COUNT; i1++)
                    strncpy_P(buffer[i1],(char*)pgm_read_dword(&(WIFI_SSIDS[i1])),WL_SSID_MAX_LENGTH + 1);
                for(i1 = 0; i1 < found; i1++)
                {
                    strncpy(name,WiFi.SSID(i1).c_str(),WL_SSID_MAX_LENGTH + 1);
                    for(i2 = pos; i2 < MEM_WIFI_COUNT; i2++)
                    {
                        if(buffer[i2] != nullptr)
                        {
                            Serial.printf("Comparing \"%s\" & \"%s\": ",name,buffer[i2]);
                            if(strcmp(name,buffer[i2]) == 0)
                            {
                                Serial.println("found!");
                                char password[WIFI_SIZE_PASSWORD + 1];
                                strncpy_P(password,(char*)pgm_read_dword(&(WIFI_PASSWORDS[i2])),WIFI_SIZE_PASSWORD + 1);
                                Serial.printf("\tname: %s, password: %s\n",name,password);
                                WiFi.mode(WIFI_STA);
                                WiFi.begin(name,password);
                                status = WIFI_CONNECTING;
                                timer_set_rel(id,WIFI_INTERVAL_CONNECTING * 1000,nullptr);
                                /*buffer[i2] = nullptr;
                                found1[f1] = i2;
                                f1++;*/
                                i1 = found;
                                break;
                            }
                            else
                                Serial.println("not found.");
                        }
                    }
                }
                if(i2)
                {
                    pos = i2 + 1;
                    if(pos >= MEM_WIFI_COUNT)
                        pos = 0;
                }
            }
            else
                timer_set_rel(id,WIFI_INTERVAL_NOT_CONNECTED * 1000,nullptr);
            /*if(millis() - ms > WL_RETRY_INTERVAL)
            {
                if(wifi_selected < wifi_count)
                {
                    char ssid[WL_SSID_MAX_LENGTH + 1];
                    char password[WL_PASSWORD_MAX_LENGTH + 1];
                    strncpy_P(ssid, (char*) pgm_read_dword(&(WIFI_SSIDS[wifi_found[wifi_selected]])),WL_SSID_MAX_LENGTH + 1);
                    strncpy_P(password, (char*) pgm_read_dword(&(WIFI_PASSWORDS[wifi_found[wifi_selected]])),WL_PASSWORD_MAX_LENGTH + 1);
                    WiFi.begin(ssid,password);
                    wifi_selected++;
                    ms = millis();
                }
                else
                    scanner = NOT_SCANNING;
            }*/
            break;
        }
    }
    return 1;
}

uint8_t wifi_init(timer_id_t timer_id,wifi_connected_t connected,wifi_disconnected_t disconnected)
{
    Serial.printf("WiFi status: %d\n",WiFi.status());
    uint8_t result = -1;
    if(WiFi.status() != WL_NO_SHIELD)
    {
        Serial.printf("WiFi status: %d\n",WiFi.status());
        //WiFi.mode(WIFI_STA);
        result = (timer_set_rel(id = timer_id,5000 * 1000,&wifi_check));
        Serial.printf("WiFi status: %d\n",WiFi.status());
        Serial.printf("WiFi initialization: %d\n",result);
        callback_con = connected;
        callback_disc = disconnected;
        /*if(!result)
            timer_tick();*/
    }
    return result;
}