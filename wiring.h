//
// Created by louis on 15.08.2021.
//

#ifndef WIRING_H_
#define WIRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "./config.h"

#define WIRINGPI_SETUP 1

#if !SIMULATED_DATA
#include "./dht11.h"

#define SPI_CHANNEL 0
#define SPI_CLOCK 1000000L

#define SPI_SETUP 1 << 2
#define BME_INIT 1 << 3
#endif

#define TEMPERATURE_ALERT 30

int readMessage(int messageId, char *payload);

#endif  // WIRING_H_
