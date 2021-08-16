//
// Created by louis on 16.08.2021.
//

#include "./dht11.h"
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAXTIMINGS	85
#define DHTPIN		7
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

float two_int_to_float(int vorkomma, int nachkomma)
{
    int i, p = 0;
    float c = 0;

    i = 10E8;
    while(i > 0)
    {
        p = nachkomma / i;
        if (p > 0)
            break;
        i /= 10;
    }

    c = (float)vorkomma + ((float)nachkomma / (float)(i * 10));

    return c;
}

int read_dht11_dat(float * Temp_c__fp, float * Hum_pct__fp)
{
    int Return_status__i = 0;
    uint8_t laststate	= HIGH;
    uint8_t counter		= 0;
    uint8_t j		= 0, i;

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    pinMode( DHTPIN, OUTPUT );
    digitalWrite( DHTPIN, LOW );
    delay( 18 );
    digitalWrite( DHTPIN, HIGH );
    delayMicroseconds( 40 );
    pinMode( DHTPIN, INPUT );

    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHTPIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHTPIN );

        if ( counter == 255 )
            break;

        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ( (j >= 40) &&
    (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        *Temp_c__fp = two_int_to_float(dht11_dat[2], dht11_dat[3]);
        *Hum_pct__fp = two_int_to_float(dht11_dat[0], dht11_dat[1]);
        printf( "Data not good, skip %f, %f\n", Temp_c__fp, Hum_pct__fp );
        printf( "Humidity = %d.%d %% Temperature = %d.%d C \n", dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3]);
        Return_status__i = 1;
    }else  {
        printf( "Data not good, skip\n" );
    }
    return Return_status__i;
}
