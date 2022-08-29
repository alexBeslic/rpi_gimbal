/**
 * @file test_angles.c
 * @author Aleksandar Bešlić (alex.beslic@gmail.com)
 * @brief Reads and prints all the accel and gyro data from the sensor. And the appropriate angles.
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "accel_gyro.h"

uint8_t flag = 1;

/**
 * @brief Runs close_me when CTRL+C is prest. Sets flag to 0 to stop the app and closes evry thig properly
 * 
 * @param i not in use
 */
void close_me(int i)
{
	flag = 0;
}

int main(int argc, char **argv)
{
	signal(SIGINT, close_me);

    uint8_t ret;

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
        printf("------------------------------------------\n");
        printf("| Gyro  | %-8.4f | %-8.4f | %-8.4f |\n", GYRO_XYZ.x, GYRO_XYZ.y, GYRO_XYZ.z);
        printf("------------------------------------------\n");
        printf("| Angles| %-8.4f | %-8.4f | %-8.4f |\n", angles.x, angles.y, angles.z);
        printf("------------------------------------------\n");

        usleep(500000);
    }
    
    close_spi_bus();
}
