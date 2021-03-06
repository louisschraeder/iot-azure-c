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

/**
 * Auslesen des Temperatursensors
 * @param wert0
 * @param wert1
 * @param wert2
 * @param wert3
 * @return
 */
int read_dht11_dat(int * wert0, int * wert1, int * wert2, int * wert3)
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

        //Die ersten 3 x 8 bits werden Ignoriert
        if ( (i >= 4) && (i % 2 == 0) )
        {
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        *wert0 = dht11_dat[0];
        *wert1 = dht11_dat[1];
        *wert2 = dht11_dat[2];
        *wert3 = dht11_dat[3];
        Return_status__i = 1;
    }else  {
        printf( "Data not good, skip\n" );
    }
    return Return_status__i;
}
