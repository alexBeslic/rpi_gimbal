#include <stdio.h>
#include <stdlib.h>
#include "accel.h"


int main(int argc, char **argv)
{
    uint8_t ret;

    ret = open_i2c_bus();

    if (ret != 0)
    {
        printf("Error occurred while opening I2C bus\n");
        return EXIT_FAILURE;
    }

   /* ret = accel_config();

    if (ret != 0)
    {
        printf("Error occurred while configuring accel\n");
        return EXIT_FAILURE;
    }*/
    
    while (1)
    {
        ret = read_accel_xyz();
        printf("Status: %d\n", ret);
        printf("X: %.4f Y: %.4f Z: %.4f \n\n", ACCEL_XYZ.x, ACCEL_XYZ.y, ACCEL_XYZ.z);

        sleep(1);
    }
    
}