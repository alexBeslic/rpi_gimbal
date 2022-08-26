#include <stdio.h>
#include <stdlib.h>
#include "accel_gyro.h"

/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c accel_test_.c -lm -o who
*/

int main(int argc, char **argv)
{
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

    /*ret = calculate_error();

    if (ret != 0)
    {
        printf("Error occurred while configuring accel\n");
        return EXIT_FAILURE;
    }*/

    
    while (1)
    {
        /*ret = read_accel_xyz();
        printf("Accel status: %d\n", ret);
        printf("X: %.8f Y: %.8f Z: %.8f \n\n", ACCEL_XYZ.x, ACCEL_XYZ.y, ACCEL_XYZ.z);

        ret = read_gyro_xyz();
        printf("Gyro status: %d\n", ret);
        printf("X: %.8f Y: %.8f Z: %.8f \n\n", GYRO_XYZ.x, GYRO_XYZ.y, GYRO_XYZ.z);*/
        getAngles();
		fflush(stdout);
        //sleep(1);
    }
    
}
