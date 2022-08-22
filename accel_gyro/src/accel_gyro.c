/**
 * @file accel_gyro.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "accel_gyro.h"

static char *spi_dev_name = (char*)"/dev/spidev0.0";
static int spi_file;
static uint8_t spi_mode;
static uint8_t spi_bitsPerWord;
static uint32_t  spi_speed;

/**
 * @brief Opens SPI bus
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t open_spi_bus()
{
    uint8_t ret_val;

    spi_mode = SPI_MODE_0; // Setting mode

    spi_bitsPerWord = 8;   // Setting bits per word

    spi_speed = 8000000;  // Setting speed 8 MHz

    if ((spi_file = open(spi_dev_name, O_RDWR)) < 0)
	{
		perror("open");
		return 1;
	}

    ret_val = ioctl(spi_file, SPI_IOC_WR_MODE, &spi_mode);
    if(ret_val < 0)
    {
        perror("Could not set SPIMode (WR)...ioctl fail");
        return 1;
    }

    ret_val = ioctl(spi_file, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(ret_val < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      return 1;
    }

    ret_val = ioctl(spi_file, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(ret_val < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      return 1;
    }

    return 0;
}

/**
 * @brief Closes SPI bus
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t close_spi_bus()
{
    uint8_t ret_val;

    ret_val = close(spi_file);

    if(ret_val < 0)
    {
      perror("Could not close SPI bus...ioctl fail");
      return 1;
    }

    return 0;
}

/**
 * @brief Transfers 1B on the SPI bus
 * 
 * @param RxData Read data
 * @param TxData Data to write
 * @return uint8_t (0 - on success; 1 - on failure)
 */
void spi_read_write(uint8_t *data, int length){
    struct spi_ioc_transfer spi[length];

    for(int i=0;i<length;i++){
        memset(&spi[i],0,sizeof(struct spi_ioc_transfer));
        spi[i].tx_buf=(unsigned long)(data+i);
        spi[i].rx_buf=(unsigned long)(data+i);
        spi[i].len=1;
        spi[i].speed_hz=spi_speed;
        spi[i].bits_per_word=spi_bitsPerWord;
    }
    if(ioctl(spi_file,SPI_IOC_MESSAGE(length),spi)<0){
        perror("Cetic");
        close(spi_file);
        exit(-1);
    }
}

/**
 * @brief Configuring sensor
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t sensor_config()
{	
	int ret;
	// Upisati sve nule u registar 6B
	ret = write_registar(PWR_MGMT_1, 0x00);
    if (ret != 0)
    {
        printf("Error occurred while reading for SPI bus\n");
        return EXIT_FAILURE;
    }
    // TODO:
    return 0;
}

/**
 * @brief Reads Accel XYZ and puts them in ACCEL_XYZ
 * 
 * @return uint8_t (0 - on success; 1 - on failure; 2 - Data not ready)
 */
uint8_t read_accel_xyz()
{	
	uint8_t data[12];
	uint16_t temp;
    data[0]=0x80 | ACCEL_XOUT_H_ADDR;
    data[1]=0x00;
    spi_read_write(data, 2);
    data[2]=0x80 | (ACCEL_XOUT_H_ADDR+1);
    data[3]=0x00;
    spi_read_write(data+2, 2);
    data[4]=0x80 | (ACCEL_XOUT_H_ADDR+2);
    data[5]=0x00;
    spi_read_write(data+4, 2);
    data[6]=0x80 | (ACCEL_XOUT_H_ADDR+3);
    data[7]=0x00;
    spi_read_write(data+6, 2);
    data[8]=0x80 | (ACCEL_XOUT_H_ADDR+4);
    data[9]=0x00;
    spi_read_write(data+8, 2);
    data[10]=0x80 | (ACCEL_XOUT_H_ADDR+5);
    data[11]=0x00;
    spi_read_write(data+10, 2);
    temp = data[1];
    printf("data1 = %x\n", temp);
    int16_t x_data = (data[1] << 8) | data[3];
    printf("x_data = %x\n", x_data);
    temp = data[5];
    printf("data5 = %x\n", temp);
    int16_t y_data = (data[5] << 8) | data[7];
    printf("y_data = %x\n", y_data);
    temp = data[9];
    printf("data9 = %x\n", temp);
    int16_t z_data = (data[9] << 8) | data[11];
    printf("z_data = %x\n", z_data);

	ACCEL_XYZ.x = (x_data / CONV_CONST);
    ACCEL_XYZ.y = (y_data / CONV_CONST);
    ACCEL_XYZ.z = (z_data / CONV_CONST);
    return 0;
}

/**
 * @brief Reads Gyro XYZ and puts them in GYRO_XYZ
 * 
 * @return uint8_t (0 - on success; 1 - on failure; 2 - Data not ready)
 */
uint8_t read_gyro_xyz()
{
    return 0;
}

/**
 * @brief Who am I 
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
 uint8_t read_whoami()
{
    uint8_t data[2];
    data[0]=0x80 | WHOAMI_ADDR;
    data[1]=0xff;
    spi_read_write(data, 2);

    printf("What is in here: %x\n", data[0]);
    printf("Who am I: %x\n", data[1]);

    return 0;
}

uint8_t read_registar(unsigned char reg)
{
    uint8_t data[2];
    data[0]=0x80 | reg;
    data[1]=0xff;
    spi_read_write(data, 2);

    printf("What is in here: %x\n", data[0]);
    printf("Who am I: %x\n", data[1]);

    return 0;
}
uint8_t write_registar(unsigned char reg, unsigned char val)
{
    uint8_t data[2];
    data[0]=0x00 | reg;
    data[1]=val;
    spi_read_write(data, 2);

    printf("What is in here: %x\n", data[0]);
    printf("Who am I: %x\n", data[1]);

    return 0;
}
