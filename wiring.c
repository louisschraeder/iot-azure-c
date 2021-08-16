//
// Created by louis on 15.08.2021.
//

#include "./wiring.h"

static unsigned int BMEInitMark = 0;

#if SIMULATED_DATA
float random(int min, int max)
{
    int range = (int)(rand()) % (100 * (max - min));
    return min + (float)range / 100;
}

int readMessage(int messageId, char *payload)
{
    float temperature = random(20, 30);
    snprintf(payload,
             BUFFER_SIZE,
             "{ \"deviceId\": \"myDevice\", \"messageId\": %d, \"temperature\": %f, \"humidity\": %f }",
             messageId,
             temperature,
             random(60, 80));
    return (temperature > TEMPERATURE_ALERT) ? 1 : 0;
}

#else
int mask_check(int check, int mask)
{
    return (check & mask) == mask;
}

// check the BMEInitMark value is equal to the (WIRINGPI_SETUP | SPI_SETUP | BME_INIT)

int readMessage(int messageId, char *payload)
{
    float temperature, humidity;
    if (read_dht11_dat(&temperature, &humidity) != 1)
    {
        return -1;
    }

    snprintf(payload,
             BUFFER_SIZE,
             "{ \"deviceId\": \"Raspberry Pi - C\", \"messageId\": %d, \"temperature\": %f, \"humidity\": %f }",
             messageId,
             temperature,
             humidity);
    return temperature > TEMPERATURE_ALERT ? 1 : 0;
}
#endif

