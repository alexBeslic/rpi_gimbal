#include <stdio.h>
#include <stdlib.h>
#include "accel_gyro.h"

/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c test_whoami.c -o who
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
    
    sensor_config();
    
    if(argc == 3)
    {
		ret = write_registar((unsigned char)atoi(argv[1]), (unsigned char)atoi(argv[2]));
		sleep(1);
	}

    if(argc == 1)
    {
        ret = read_registar(WHOAMI_ADDR);
    }
    else
    {
        ret = read_registar((unsigned char)atoi(argv[1]));
    }
    

    if (ret != 0)
    {
        printf("Error occurred while reading for SPI bus\n");
        return EXIT_FAILURE;
    }

    ret = close_spi_bus();

    if (ret != 0)
    {
        printf("Error occurred while reading for SPI bus\n");
        return EXIT_FAILURE;
    }

    return 0;
}
