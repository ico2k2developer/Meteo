//
// Created by ico2k2 on 21/5/2023.
//

#include <wifi.hpp>

uint8_t connected = 0;

uint8_t wifi_check(timer_trigger_t trigger)
{
    if(WiFi.isConnected())
    {
        if(!connected)
        {
            connected = 1;
            Serial.printf("Connected to %s\n",WiFi.SSID().c_str());
            configTime(TZ,NTP_SERVER_1,NTP_SERVER_2,NTP_SERVER_3);
            timer_set_rel(timer_id,1000 * 1000,)
        }
        else
        {

            /*if(millis() - ms > 1000)
            {
                time_t data;
                time(&data);
                struct tm * info = localtime(&data);
                char tmp[100];
                strftime(tmp,100,"%A, %d %B %G, %H:%M:%S\n",info);
                Serial.print(tmp);
                ms = millis();
            }*/
        }
    }
    else
    {
        switch(scanner)
        {
            case NOT_SCANNING:
            {
                WiFi.disconnect(false);
                WiFi.scanNetworksAsync(&wifiScanResult,true);
                scanner = SCANNING;
                wifi_selected = 0;
                break;
            }
            case SCANNING:
            {
                break;
            }
            case SCAN_ENDED:
            {
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
        connected = 0;
        sntp_stop();
    }
}

uint8_t wifi_init(timer_id_t timer_id)
{
    return timer_set_rel(timer_id,5000 * 1000,&wifi_check);
}

void wifi_scan_result(int foundCount)
{
    if(foundCount > 0)
    {
        uint8_t i1,i2;
        char name[WL_SSID_MAX_LENGTH + 1];
        char buffer[MEM_WIFI_COUNT][WL_SSID_MAX_LENGTH + 1];
        memset(wifi_found,0,sizeof(uint8_t) * MEM_WIFI_COUNT);
        wifi_count = 0;
        for(i1 = 0; i1 < MEM_WIFI_COUNT; i1++)
        {
            strncpy_P(buffer[i1],(char*) pgm_read_dword(&(WIFI_SSIDS[i1])), WL_SSID_MAX_LENGTH + 1);
        }
        for(i1 = 0; i1 < foundCount; i1++)
        {
            strncpy(name,WiFi.SSID(i1).c_str(),WL_SSID_MAX_LENGTH + 1);
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
        scanner = NOT_SCANNING;
}