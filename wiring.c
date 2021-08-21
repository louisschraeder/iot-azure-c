//
// Created by louis on 15.08.2021.
//

#include "./wiring.h"

static unsigned int BMEInitMark = 0;




int mask_check(int check, int mask)
{
    return (check & mask) == mask;
}



int readMessage(int messageId, char *payload)
{
    struct wiringPiNodeStruct *node;

    if ( wiringPiSetup() == -1 )
    {
        printf( "wire setup fail\n" );
        return -1;
    }

    int analogEarth;
    ads1115Setup(120,0x48, node);
    analogEarth = myAnalogRead(node, 0);

    printf( "analog: %d0\n",  analogEarth);

    int earth, rain;
    readEarth(&earth);
    readRain(&rain);


    int wert0, wert1, wert2, wert3;
    if (read_dht11_dat(&wert0, &wert1, &wert2, &wert3) != 1)
    {
        printf( "wire read fail\n" );
        return -1;
    }

    snprintf(payload,
             BUFFER_SIZE,
             "{ \"deviceId\": \"myDevice\", \"messageId\": %d, \"temperature\": %d.%d, \"humidity\": %d.%d, \"earth\": %d, \"rain\": %d }",
             messageId,
             wert2, wert3,
             wert0, wert1,
             earth, rain);


    return 0;
}


