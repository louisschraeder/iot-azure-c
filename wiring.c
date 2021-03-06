//
// Created by louis on 15.08.2021.
//

#include "./wiring.h"

static unsigned int BMEInitMark = 0;




int mask_check(int check, int mask)
{
    return (check & mask) == mask;
}


/**
 * Erstellen der Message für die Cloud
 * @param messageId
 * @param payload
 * @return
 */
int readMessage(int messageId, char *payload)
{
    if ( wiringPiSetup() == -1 )
    {
        printf( "wire setup fail\n" );
        return -1;
    }

    int wert0, wert1, wert2, wert3;
    if (read_dht11_dat(&wert0, &wert1, &wert2, &wert3) != 1)
    {
        printf( "temp read fail\n" );
        return -1;
    }

    int earth, rain;
    if (readAnalog(&earth, &rain) != 1)
    {
        printf( "earth/rain read fail\n" );
        return -1;
    }

    snprintf(payload,
             BUFFER_SIZE,
             "{ \"deviceId\": \"SensorA\", \"messageId\": %d, \"temperature\": %d.%d, \"humidity\": %d.%d, \"earth\": %d, \"rain\": %d }",
             messageId,
             wert2, wert3,
             wert0, wert1,
             earth, rain);


    return 0;
}


