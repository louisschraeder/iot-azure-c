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
    //wiringPiSetupGpio();
    pinMode (gpioEarth, INPUT);
    //pullUpDnControl(gpioEarth,PUD_UP);
    int value = 0;

    //value = digitalRead(gpioEarth);
    while(1) {
        if (digitalRead(gpioEarth) == HIGH) {
            printf("Button pressed\n");
            delay(200);
        }
        delay(1);
    }
    printf( "Earth: %d\n", value);

    return 0;
}