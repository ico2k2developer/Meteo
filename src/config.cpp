//
// Created by ico2k2 on 25/9/2022.
//

#include "config.h"

void syncTime(NTPClient client,uRTCLib rtc)
{
    unsigned long epoch = client.getEpochTime() + NTP_TIMEZONE * 60 * 60;
    uint16_t quartet = epoch / (60 * 60 * 24 * (365 * 4 + 1));
    uint16_t day = (epoch - quartet * (60 * 60 * 24 * (365 * 4 + 1))) / (60 * 60 * 24);
    uint16_t year;
    if(day < (365 * 2))
        year = day / 365;
    else
        year = day < (365 * 3 + 1) ? 2 : 3;
    day -= year * 365;
    if(year > 2)
        day--;
    uint8_t mday = 1,month = 0,i;
    uint16_t tmp,last = 0;
    for(i = 0; i < (sizeof(DAY0S) / sizeof(DAY0S[0])); i++)
    {
        tmp = DAY0S[i];
        if(year == 2)
        {
            if(i == 1)
                tmp++;
            else if(i == 2)
                day--;
        }
        if(day < tmp)
        {
            month = i + 1;
            mday += day - last;
            break;
        }
        last = tmp;
    }
    year += quartet * 4 - 30;

    uint8_t wk = client.getDay();
    if(wk == 0)
        wk = 7;

    rtc.set(
            client.getSeconds(),client.getMinutes(),
            client.getHours() + NTP_TIMEZONE,wk,mday,month,year);

    Serial.printf("Current time is %d/%d/%d %d:%d:%d\n",
                  rtc.day(),rtc.month(),rtc.year() + 2000,
                  rtc.hour(),rtc.minute(),rtc.second());
}


/*------------------------------------------------------
 * FUNCTION - I2C_ClearBus
 * (http://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html)
 * (c)2014 Forward Computing and Control Pty. Ltd.
 * NSW Australia, www.forward.com.au
 * This code may be freely used for both private and commercial use
 *
 * This routine turns off the I2C bus and clears it
 * on return SCA and SCL pins are tri-state inputs.
 * You need to call Wire.begin() after this to re-enable I2C
 * This routine does NOT use the Wire library at all.
 *
 * returns 0 if bus cleared
 *         1 if SCL held low.
 *         2 if SDA held low by slave clock stretch for > 2sec
 *         3 if SDA held low after 20 clocks.
 *
 *  mapping for the esp8266
 *    D0 = GPIO16;
 *    D1 = GPIO5; this is the default for wire for SCL
 *    D2 = GPIO4; confirmed with test as how i2c setup = SDA
 *                this is also the default for wire
 *    D3 = GPIO0;
 *    D4 = GPIO2; on the esp8266 this is the blue LED
 *    D5 = GPIO14;
 *    D6 = GPIO12;
 *    D7 = GPIO13;
 *    D8 = GPIO15;
 *    D9 = GPIO3;
 *    D10 = GPIO1;
 *    LED_BUILTIN = GPIO16 (auxiliary constant for the board LED, not a board pin);
 --------------------------------------------------------*/
short I2C_ClearBus()
{
    pinMode(SDA, INPUT_PULLUP); /* Make SDA (data) and SCL (clock) pins Inputs with pullup. */
    pinMode(SCL, INPUT_PULLUP);

    delay(2500);  /* Wait 2.5 secs. This is strictly only necessary on the first power
                 * up of the DS3231 module to allow it to initialize properly,
                 * but is also assists in reliable programming of FioV3 boards as it gives the
                 * IDE a chance to start uploaded the program  before existing sketch
                 * confuses the IDE by sending Serial data. */

    boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
    if (SCL_LOW) { return 1; } /* I2C bus error. Could not clear SCL clock line held low */
    /* If it is held low Arduno cannot become the I2C master. */
    boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
    int clockCount = 20; // > 2x9 clock
    while (SDA_LOW && (clockCount > 0)) { /*  vii. If SDA is Low, */
        clockCount--;
        /* Note: I2C bus is open collector so do NOT drive SCL or SDA high. */
        pinMode(SCL, INPUT);   /* release SCL pullup so that when made output it will be LOW */
        pinMode(SCL, OUTPUT);  /* then clock SCL Low */
        delayMicroseconds(10); /* for >5uS */
        pinMode(SCL, INPUT); // release SCL LOW
        pinMode(SCL, INPUT_PULLUP); /* turn on pullup resistors again */
        /* do not force high as slave may be holding it low for clock stretching. */
        delayMicroseconds(10); /* for >5uS */
        /* The >5uS is so that even the slowest I2C devices are handled. */
        SCL_LOW = (digitalRead(SCL) == LOW); /* Check if SCL is Low. */
        int counter = 20;
        while (SCL_LOW && (counter > 0)) {  /* loop waiting for SCL to become High only wait 2sec. */
            counter--;
            delay(100);
            SCL_LOW = (digitalRead(SCL) == LOW); }
        if (SCL_LOW) { return 2; }
        /* still low after 2 sec error I2C bus error. Could not clear. SCL clock line
         * held low by slave clock stretch for >2sec */
        SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
    } /* end while */
    if (SDA_LOW) { return 3; }
    /* still low,I2C bus error. Could not clear. SDA data line held low */

    /* else pull SDA line low for Start or Repeated Start */
    pinMode(SDA, INPUT); /* remove pullup. */
    pinMode(SDA, OUTPUT);  /* and then make it LOW i.e. send an I2C Start or Repeated start control.
                          * When there is only one I2C master a Start or Repeat Start has the same
                          * function as a Stop and clears the bus. A Repeat Start is a Start
                          * occurring after a Start with no intervening Stop. */
    delayMicroseconds(10); /* wait >5uS */
    pinMode(SDA, INPUT);   /* remove output low */
    pinMode(SDA, INPUT_PULLUP); /* and make SDA high i.e. send I2C STOP control. */
    delayMicroseconds(10);      /* x. wait >5uS */
    pinMode(SDA, INPUT);   /* and reset pins as tri-state inputs which is the default state on reset */
    pinMode(SCL, INPUT);
    return 0; /* all ok */
} /* end function I2C_ClearBus() */
////////////////////////////////////////////////////////////////////////////////////////////////////