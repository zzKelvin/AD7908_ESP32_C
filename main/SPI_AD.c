#include "SPI_AD.h"

esp_err_t err;
spi_device_handle_t spi;

esp_err_t spi_init(){
	esp_err_t err;
    spi_bus_config_t buscfg={
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    };
    err = spi_bus_initialize(VSPI_HOST, &buscfg, 0);
    return err;
}

esp_err_t add_slave(spi_device_handle_t* spi){
	esp_err_t err;
    spi_device_interface_config_t devcfg={
    	.command_bits = 0,
		.address_bits = 0,
        .clock_speed_hz = 30000,
        .mode = 2,
        .spics_io_num = PIN_NUM_CS,
        .queue_size=1,
    };
    err = spi_bus_add_device(VSPI_HOST, &devcfg, spi);
    return err;
}

esp_err_t read_data(spi_device_handle_t spi, uint16_t *data_receive, uint16_t data_send)
{
    esp_err_t err;
    spi_transaction_t t;
    spi_transaction_t *t_res;
    t_res = &t;
    memset(&t, 0, sizeof(t));
    t.length = 16;
    t.tx_buffer = &data_send;
    t.rxlength = 16;
    t.rx_buffer = data_receive;
    t.cmd = NULL;
    t.addr = NULL;
    t.user = (void*)1;
    err = spi_device_queue_trans(spi, (spi_transaction_t*)&t, 100);
    err = spi_device_get_trans_result(spi, (spi_transaction_t**)&t_res, 100);
    return err;
}

uint16_t SPI_Read(uint16_t Address)
{
    uint16_t returnData = 0; 
    read_data(spi, &returnData, Address); 
    returnData = SPI_SWAP_DATA_RX(returnData, 16); 
	return returnData;
}

void Read_All_Sensors()
{
    float vin[8] = {};
    SPI_Read(CLEAR_INITIAL_READ);
    SPI_Read(CONFIG);
    SPI_Read(CHANNELS);

    while(true)
    {
        vin[0] = ((SPI_Read(0x00)>>4)&0xff);
        vin[1] = ((SPI_Read(0x00)>>4)&0xff); 
        vin[2] = ((SPI_Read(0x00)>>4)&0xff);
        vin[3] = ((SPI_Read(0x00)>>4)&0xff); 
        vin[4] = ((SPI_Read(0x00)>>4)&0xff); 
        vin[5] = ((SPI_Read(0x00)>>4)&0xff); 
        vin[6] = ((SPI_Read(0x00)>>4)&0xff);
        vin[7] = ((SPI_Read(0x00)>>4)&0xff);

        printf("SENSOR (1): %f\n",   vin[0]); 
        printf("SENSOR (2): %f\n",   vin[1]); 
        printf("SENSOR (3): %f\n",   vin[2]); 
        printf("SENSOR (4): %f\n",   vin[3]); 
        printf("SENSOR (5): %f\n",   vin[4]); 
        printf("SENSOR (6): %f\n",   vin[5]); 
        printf("SENSOR (7): %f\n",   vin[6]); 
        printf("SENSOR (8): %f\n\n", vin[7]); 
        vTaskDelay(250/portTICK_PERIOD_MS);
    }
}

void init_spi(void)
{
    err = spi_init();
	err = add_slave(&spi);
}