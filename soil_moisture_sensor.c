//
// Created by louis on 17.08.2021.
//

#include "./soil_moisture_sensor.h"
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define gpioEarth 0

int readEarth()
{
    pinMode (gpioEarth, INPUT);

    if (digitalRead(gpioEarth) == LOW) {
        printf("trocken\n");
        delay(200);
    }
    else
    {
        printf("nass\n");
        delay(200);
    }

    return 0;
}