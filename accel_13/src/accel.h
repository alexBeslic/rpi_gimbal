#ifndef ACCEL_H_INCLUDED
#define ACCEL_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/* Sensor addreses */
#define SAD				0x30 //<<< I2C slave address
#define GYRO_CTRL_ADDR 		0x1B
#define ACCEL_CTRL1_ADDR	0x1C
#define ACCEL_CTRL2_ADDR	0x1D
#define ACCEL_XOUT_H        0x3B

/* Control data */
#define CTRL1_REG		0x74 //<< High-Perf 400 Hz, 14 bit 
#define CTRL6_REG		0x04 //<< Low Noise, 2 g

/* Coversion const*/
#define CONV_CONST		0.244f

char *filename = (char*)"/dev/i2c-1";

struct accel
{
    float x;
    float y;
    float z;
};

volatile struct accel ACCEL_XYZ;

uint8_t open_i2c_bus();
uint8_t accel_config();
uint8_t read_accel_xyz();

#endif // ACCEL_H_INCLUDED