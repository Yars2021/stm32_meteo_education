#define MAIN

#include "include.h"

uint8_t message;

int main (void)
{
    initLowLevel();

    char * buf = "Hello";

    transiver trans;

    init_trans(&trans, 0);
    trans.output = buf;
    trans.output_size = sizeof(buf);
    
    while (1)
    {
        transmite(&trans);
        HAL_Delay(1000);
    }
}
