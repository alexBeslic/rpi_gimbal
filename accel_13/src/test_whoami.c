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

    ret = read_whoami();

    if (ret != 0)
    {
        printf("Error occurred while opening I2C bus\n");
        return EXIT_FAILURE;
    }

    return 0;
}