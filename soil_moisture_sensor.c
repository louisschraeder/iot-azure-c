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
    printf("1");
    pinMode(gpioEarth, INPUT);
    printf("2");
    while(1)
    {
        if (digitalRead(gpioEarth))
        {
            printf("nass");
        }
        else
        {
            printf("trocken");
        }
        //value = digitalRead(gpioEarth);

        //printf( "Earth: %d\n", value);
        printf("3");
        delay (1000) ;
    }

    return 0;
}