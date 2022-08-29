/**
 * @file accel_gyro.h
 * @author Aleksandar Bešlić (alex.beslic@gmail.com)
 * @brief Header file for the library for 6DOF IMU 5 CLICK(ICM-20789). Uses SPI bus to interface with the sensor.
 * @version 0.8
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef ACCEL_GYRO_H_INCLUDED
#define ACCEL_GYRO_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/time.h>

#define READ_BIT(b) ((0x80) | (b)) /* Sets the fisrt bit to 1*/

/* Register Address*/

#define WHOAMI_ADDR         0x75 // 8-bit device ID (0x03)
#define GYRO_CTRL_ADDR 		0x1B
#define ACCEL_CTRL1_ADDR	0x1C
#define ACCEL_CTRL2_ADDR	0x1D

#define ACCEL_XOUT_H_ADDR   0x3B
#define ACCEL_XOUT_L_ADDR   0x3C
#define ACCEL_YOUT_H_ADDR   0x3D
#define ACCEL_YOUT_L_ADDR   0x3E
#define ACCEL_ZOUT_H_ADDR   0x3F
#define ACCEL_ZOUT_L_ADDR   0x40

#define GYRO_XOUT_H_ADDR    0x43
#define GYRO_XOUT_L_ADDR    0x44
#define GYRO_YOUT_H_ADDR    0x45
#define GYRO_YOUT_L_ADDR    0x46
#define GYRO_ZOUT_H_ADDR    0x47
#define GYRO_ZOUT_L_ADDR    0x48

#define PWR_MGMT_1_ADDR 	0x6B

/* Register Configuration*/

#define ACCEL_CTRL1_CONF    0x18 // Full scale 16 g
#define GYRO_CTRL_CONF      0x18 // Full scale 2000 dps

/* Coversion const*/

#define ACCEL_CONV_CONST	2048.00 // Units LSB/g 
#define GYRO_CONV_CONST     16.4 // Units LSB/(dps)

// Struct definition

struct sensor_data
{
    float x;
    float y;
    float z;
};

volatile struct sensor_data ACCEL_XYZ; // Accel XYZ data (g)
volatile struct sensor_data GYRO_XYZ;  // Gyro XYZ data (dsp)
volatile struct sensor_data angles;    // XYZ angels (in degrees)



// Function declaration

uint8_t open_spi_bus();
uint8_t close_spi_bus();
uint8_t sensor_config();
uint8_t read_accel_xyz();
uint8_t read_gyro_xyz();
uint8_t read_whoami();
uint8_t read_registar(unsigned char);
uint8_t write_registar(unsigned char, unsigned char);
uint8_t calculate_error();
long long current_time();
void getAngles();

#endif
