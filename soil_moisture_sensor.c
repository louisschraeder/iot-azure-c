//
// Created by louis on 17.08.2021.
//

#include "./soil_moisture_sensor.h"
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define gpioEarth 0
#define gpioRain 2

int readEarth(int * earth)
{
    pinMode (gpioEarth, INPUT);

    if (digitalRead(gpioEarth) == LOW) {
        *earth = 0;
    }
    else
    {
        *earth = 1;
    }



    return 0;
}

int readRain(int * rain)
{
    pinMode (gpioRain, INPUT);

    if (digitalRead(gpioRain) == LOW) {
        *rain = 0;
    }
    else
    {
        *rain = 1;
    }

    return 0;
}

