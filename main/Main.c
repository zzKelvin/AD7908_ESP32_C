#include <stdio.h>
#include "SPI_AD.h"

void app_main(void)
{
    init_spi();
    Read_All_Sensors();
}