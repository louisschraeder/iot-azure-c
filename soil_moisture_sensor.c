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

    if(wiringPiSetup() == -1){
        printf("setup wiringPi failed !");
        return 1;
    }

    pinMode( gpioEarth, INPUT );

    for(;;)
    {
        value = analogRead(gpioEarth);

        printf( "Earth: %d\n", value);

        delay (1000) ;
    }

    return 0;
}