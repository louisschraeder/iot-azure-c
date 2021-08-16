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

int read_dht11_dat(float * Temp_c__fp, loat * Hum_pct__fp)
{
    uint8_t laststate	= HIGH;
    uint8_t counter		= 0;
    uint8_t j		= 0, i;
    float	f;

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
        f = dht11_dat[2] * 9. / 5. + 32;
        *Temp_c__fp = two_int_to_float(dht11_dat[0], dht11_dat[1]);
        *Hum_pct__fp = two_int_to_float(dht11_dat[2], dht11_dat[3]);
        printf( "Humidity = %d.%d %% Temperature = %d.%d C (%.1f F)\n",
                dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
    }else  {
        printf( "Data not good, skip\n" );
    }
}

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