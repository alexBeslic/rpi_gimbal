#ifndef ACCEL_GYRO_H_INCLUDED
#define ACCEL_GYRO_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define READ_BIT(b) ((0x80) | (b)) /* Sets the fisrt bit to 1*/

#define WHOAMI_ADDR         0x75 // 8-bit device ID (0x03)
#define GYRO_CTRL_ADDR 		0x1B
#define ACCEL_CTRL1_ADDR	0x1C
#define ACCEL_CTRL2_ADDR	0x1D
#define ACCEL_XOUT_H_ADDR   0x3B // 6B to read
#define GYRO_XOUT_H_ADDR    0x43 // 6B to read

/* Coversion const*/
#define CONV_CONST		16.384 // Units LSB/g 

// Strict definition
struct sensor_data
{
    float x;
    float y;
    float z;
};

volatile struct sensor_data ACCEL_XYZ;
volatile struct sensor_data GYRO_XYZ;

// Function declaration
uint8_t open_spi_bus();
uint8_t close_spi_bus();
uint8_t sensor_config();
uint8_t read_accel_xyz();
uint8_t read_gyro_xyz();
uint8_t read_whoami();

#endif