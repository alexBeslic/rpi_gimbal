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

    ret_val = ioctl(spi_file, SPI_IOC_RD_MODE, &spi_mode);
    if(ret_val < 0)
    {
      perror("Could not set SPIMode (RD)...ioctl fail");
      return 1;
    }

    ret_val = ioctl(spi_file, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
    if(ret_val < 0)
    {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      return 1;
    }

    ret_val = ioctl(spi_file, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
    if(ret_val < 0)
    {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      return 1;
    }

    ret_val = ioctl(spi_file, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if(ret_val < 0)
    {
      perror("Could not set SPI speed (WR)...ioctl fail");
      return 1;
    }

    ret_val = ioctl(spi_file, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if(ret_val < 0)
    {
      perror("Could not set SPI speed (RD)...ioctl fail");
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
static uint8_t spi_read_write(uint8_t *RxData, uint8_t *TxData)
{
  uint8_t ret_val;
  struct spi_ioc_transfer spi;

  // Setting the struct 0 for safety
  memset(&spi, 0, sizeof(struct spi_ioc_transfer));


  spi.tx_buf = (unsigned long)TxData;		//transmit from "data"
	spi.rx_buf = (unsigned long)RxData;		//receive into "data"
	spi.len = 1;
	spi.delay_usecs = 0;
	spi.speed_hz = spi_speed;
	spi.bits_per_word = spi_bitsPerWord;

	ret_val = ioctl(spi_file, SPI_IOC_MESSAGE(1), &spi);

	if(ret_val < 0)
	{
		perror("Error - Problem transmitting spi data..ioctl");
		return 1;
	}

    return 0;
}

/**
 * @brief Configuring sensor
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t sensor_config()
{
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
    // TODO:
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
    uint8_t ret_val;
    uint8_t addr = 0x80 | WHOAMI_ADDR; // Sets the MSB to 1 for read mode
    uint8_t data;

    ret_val = spi_read_write(&data, &addr);

    if(ret_val < 0)
    {
      perror("Could not read for SPI bus...ioctl fail");
      return 1;
    }

    printf("What is in here: %d", addr);
    printf("Who am I: %d", data);

    return 0;
}