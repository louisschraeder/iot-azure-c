//
// Created by louis on 17.08.2021.
//

#include "./soil_moisture_sensor.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define gpioEarth 11

int readEarth()
{
    int value = 0;

    wiringPiSetup () ;

    for(;;)
    {
        value = analogRead(gpioEarth + 0);

        printf( "Earth: %d\n", value);

        delay (1000) ;
    }

    return 0;
}