#ifndef SPI_AD_H
#define SPI_AD_H

    #include <string.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/queue.h>
    #include <esp_err.h>
    #include <driver/spi_master.h>
    #include <driver/spi_common.h>

    //SET YOUR PINS
    #define PIN_NUM_MISO 23 
    #define PIN_NUM_MOSI 19
    #define PIN_NUM_CLK  18
    #define PIN_NUM_CS   5

    //necessary for first read
    #define CLEAR_INITIAL_READ 0Xffff
    //Config in datasheet, PM0,PM1,SHADOW,SEQ etc.
    #define CONFIG ((uint16_t)0b1000001110110000) 
    //Channels, two sequences to 8 channels, all channels activated, set 0 to deactivate channel
    #define CHANNELS ((uint16_t)0b1111111111111111)

    void Read_All_Sensors();
    void init_spi(void);
    uint16_t SPI_Read(uint16_t Address);

#endif