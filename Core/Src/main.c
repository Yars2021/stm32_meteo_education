#define MAIN

#include "include.h"

uint8_t message;

int main (void)
{
    initLowLevel();

    char * buf = "Hello";
    
    while (1)
    {
        begin_packet();
		transmite(buf, sizeof(buf));
		end_packet();
        HAL_Delay(1000);
    }
}
