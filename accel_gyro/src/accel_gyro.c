/**
 * @file accel_gyro.c
 * @author rpi_gimbal
 * @brief SPI interface for 6DOF IMU 5 CLICK(ICM-20789).
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
static struct sensor_data ACCEL_ERROR = {0.00037992, -0.20791744, 0.00};
static struct sensor_data GYRO_ERROR = {0.34698471, 0.64114493, -0.28187019};
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
* @brief Transfers data on the SPI interface
 * 
 * @param data Write/Read data
 * @param length Number of bytes to transfers
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t spi_read_write(uint8_t *data, int length)
{
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
        perror("Could not write/read SPI bus...ioctl fail");
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
	uint8_t ret_val;

    // Write all zeros to POWER MANAGEMENT 1 (Sleep mode off)
	ret_val = write_registar(PWR_MGMT_1_ADDR, 0x00);

    if (ret_val != 0)
    {
        printf("Error occurred while reading for SPI bus\n");
        return EXIT_FAILURE;
    }

    return 0;
}

/**
 * @brief Reads Accel XYZ and puts them in ACCEL_XYZ
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t read_accel_xyz()
{	
    uint8_t ret_val;
    uint8_t dummy_val = 0x69;
	uint8_t data[12] = {0};

    data[0] = 0x80 | ACCEL_XOUT_H_ADDR;
    data[1] = dummy_val;
    ret_val = spi_read_write(data, 2);

    data[2] = 0x80 | (ACCEL_XOUT_H_ADDR+1);
    data[3] = dummy_val;
    ret_val = spi_read_write(data+2, 2);

    data[4] = 0x80 | (ACCEL_XOUT_H_ADDR+2);
    data[5] = dummy_val;
    ret_val = spi_read_write(data+4, 2);

    data[6] = 0x80 | (ACCEL_XOUT_H_ADDR+3);
    data[7] = dummy_val;
    ret_val = spi_read_write(data+6, 2);

    data[8] = 0x80 | (ACCEL_XOUT_H_ADDR+4);
    data[9] = dummy_val;
    ret_val = spi_read_write(data+8, 2);

    data[10] = 0x80 | (ACCEL_XOUT_H_ADDR+5);
    data[11] = dummy_val;
    ret_val = spi_read_write(data+10, 2);

    if (ret_val != 0)
    {
        printf("Error occurred while reading Accel sensor\n");
        return 1;
    }

    int16_t x_data = (data[1] << 8) | data[3];
    int16_t y_data = (data[5] << 8) | data[7];
    int16_t z_data = (data[9] << 8) | data[11];

	ACCEL_XYZ.x = (x_data / ACCEL_CONV_CONST);
    ACCEL_XYZ.y = (y_data / ACCEL_CONV_CONST);
    ACCEL_XYZ.z = (z_data / ACCEL_CONV_CONST);

    return 0;
}

/**
 * @brief Reads Gyro XYZ and puts them in GYRO_XYZ
 * 
 * @return uint8_t (0 - on success; 1 - on failure; 2 - Data not ready)
 */
uint8_t read_gyro_xyz()
{
    uint8_t ret_val;
    uint8_t dummy_val = 0x69;
	uint8_t data[12] = {0};

    data[0] = 0x80 | GYRO_XOUT_H_ADDR;
    data[1] = dummy_val;
    ret_val = spi_read_write(data, 2);

    data[2] = 0x80 | (GYRO_XOUT_H_ADDR+1);
    data[3] = dummy_val;
    ret_val = spi_read_write(data+2, 2);

    data[4] = 0x80 | (GYRO_XOUT_H_ADDR+2);
    data[5] = dummy_val;
    ret_val = spi_read_write(data+4, 2);

    data[6] = 0x80 | (GYRO_XOUT_H_ADDR+3);
    data[7] = dummy_val;
    ret_val = spi_read_write(data+6, 2);

    data[8] = 0x80 | (GYRO_XOUT_H_ADDR+4);
    data[9] = dummy_val;
    ret_val = spi_read_write(data+8, 2);

    data[10] = 0x80 | (GYRO_XOUT_H_ADDR+5);
    data[11] = dummy_val;
    ret_val = spi_read_write(data+10, 2);

    if (ret_val != 0)
    {
        printf("Error occurred while reading Gyro sensor\n");
        return 1;
    }

    int16_t x_data = (data[1] << 8) | data[3];
    int16_t y_data = (data[5] << 8) | data[7];
    int16_t z_data = (data[9] << 8) | data[11];

	GYRO_XYZ.x = (x_data / GYRO_CONV_CONST);
    GYRO_XYZ.y = (y_data / GYRO_CONV_CONST);
    GYRO_XYZ.z = (z_data / GYRO_CONV_CONST);

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
    uint8_t data[2];

    data[0] = 0x80 | WHOAMI_ADDR;
    data[1] = 0xff;
    ret_val = spi_read_write(data, 2);

    if (ret_val != 0)
    {
        printf("Error occurred while reading Who am I reg.\n");
        return 1;
    }

    printf("First byte: %x\n", data[0]);
    printf("Who am I: %x\n", data[1]);

    return 0;
}

/**
 * @brief Reads 1B from a register
 * 
 * @param reg 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t read_registar(unsigned char reg)
{
    uint8_t ret_val;
    uint8_t data[2];

    data[0] = 0x80 | reg;
    data[1] = 0xff;
    ret_val = spi_read_write(data, 2);

    if (ret_val != 0)
    {
        printf("Error occurred while reading Who am I reg.\n");
        return 1;
    }

    printf("Reg: %x\n", data[0]);
    printf("Value: %x\n", data[1]);

    return 0;
}

/**
 * @brief Writes 1B to a register
 * 
 * @param reg 
 * @param val 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t write_registar(unsigned char reg, unsigned char val)
{
    uint8_t ret_val;
    uint8_t data[2];

    data[0] = 0x00 | reg;
    data[1] = val;
    ret_val = spi_read_write(data, 2);

    if (ret_val != 0)
    {
        printf("Error occurred while reading Who am I reg.\n");
        return 1;
    }

    printf("Reg: %x\n", data[0]);
    printf("Value: %x\n", data[1]);

    return 0;
}

/**
 * @brief Gets the error correction values for accel and gyro
 * 
 * @return uint8_t (0 - on success; 1 - on failure)
 */
uint8_t calculate_error()
{
    uint8_t i;
    uint8_t ret_val;

    while (i < 200)
    {
        ret_val = read_accel_xyz();

        if (ret_val != 0)
        {
            printf("Error occurred while reading accel regs.\n");
            return 1;
        }
                                        
        ACCEL_ERROR.x = ACCEL_ERROR.x + ((atan(ACCEL_XYZ.y) / sqrt(pow(ACCEL_XYZ.x, 2) + pow(ACCEL_XYZ.z, 2)))) / 180 * M_PI;
        ACCEL_ERROR.y = ACCEL_ERROR.y + ((atan(-1 * (ACCEL_XYZ.x) / sqrt(pow((ACCEL_XYZ.y), 2) + pow((ACCEL_XYZ.z), 2))) * 180 / M_PI));

        i++;
    }

    ACCEL_ERROR.x = ACCEL_ERROR.x / 200;
    ACCEL_ERROR.y = ACCEL_ERROR.y / 200;

    i = 0;

    while (i < 200)
    {
        ret_val = read_gyro_xyz();

        if (ret_val != 0)
        {
            printf("Error occurred while reading gyro regs.\n");
            return 1;
        }

        GYRO_ERROR.x = GYRO_ERROR.x + GYRO_XYZ.x;
        GYRO_ERROR.y = GYRO_ERROR.y + GYRO_XYZ.y;
        GYRO_ERROR.z = GYRO_ERROR.z + GYRO_XYZ.z;

        i++;
    }

    GYRO_ERROR.x = GYRO_ERROR.x / 200;
    GYRO_ERROR.y = GYRO_ERROR.y / 200;
    GYRO_ERROR.z = GYRO_ERROR.z / 200;
    
    printf("Accel error:\n");
    printf("X: %.8f Y: %.8f Z: %.8f \n\n", ACCEL_ERROR.x, ACCEL_ERROR.y, ACCEL_ERROR.z);

    printf("Gyro error:\n");
    printf("X: %.8f Y: %.8f Z: %.8f \n\n", GYRO_ERROR.x, GYRO_ERROR.y, GYRO_ERROR.z);

    return 0;
}

