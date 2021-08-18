//
// Created by louis on 17.08.2021.
//

#include "./soil_moisture_sensor.h"
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define gpioEarth 0

int readEarth(int * earth)
{
    pinMode (gpioEarth, INPUT);

    if (digitalRead(gpioEarth) == LOW) {
        *earth = 0;
        delay(200);
    }
    else
    {
        *earth = 1;
        delay(200);
    }

    return 0;
}