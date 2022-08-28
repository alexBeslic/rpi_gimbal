#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "accel_gyro.h"
/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c test_angles.c -lm -lrt -std=gnu99 -o angles

*/

uint8_t flag = 1;

void close_me(int i)
{
	flag = 0;
}

int main(int argc, char **argv)
{
    /* Runs close_me when CTRL+C is prest*/
	signal(SIGINT, close_me);

    u_int8_t ret;

    ret = open_spi_bus();

    if (ret != 0)
    {
        printf("Error occurred while opening SPI bus\n");
        return EXIT_FAILURE;
    }

	ret = sensor_config();

    if (ret != 0)
    {
        printf("Error occurred while configuring accel\n");
        return EXIT_FAILURE;
    }

    sleep(3);

    while (flag)
    {
        getAngles();
        system("clear");

        printf("------------------------------------------\n");
        printf("| Accel | %-8.4f | %-8.4f | %-8.4f |\n", ACCEL_XYZ.x, ACCEL_XYZ.y, ACCEL_XYZ.z);
        printf("------------------------------------------");
        printf("| Gyro  | %-8.4f | %-8.4f | %-8.4f |\n", GYRO_XYZ.x, GYRO_XYZ.y, GYRO_XYZ.z);
        printf("------------------------------------------");
        printf("| Angles| %-8.4f | %-8.4f | %-8.4f |\n", angles.x, angles.y, angles.z);
        printf("------------------------------------------\n");

        usleep(500000);
    }
    

    close_spi_bus();
}