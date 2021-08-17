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
    if ( wiringPiSetup() == -1 )
    {
        printf( "wire setup fail\n" );
        return -1;
    }

    //readEarth();


    int wert0, wert1, wert2, wert3;
    if (read_dht11_dat(&wert0, &wert1, &wert2, &wert3) != 1)
    {
        printf( "wire read fail\n" );
        return -1;
    }

    snprintf(payload,
             BUFFER_SIZE,
             "{ \"deviceId\": \"myDevice\", \"messageId\": %d, \"temperature\": %d.%d, \"humidity\": %d.%d }",
             messageId,
             wert2, wert3,
             wert0, wert1);


    return 0;
}


