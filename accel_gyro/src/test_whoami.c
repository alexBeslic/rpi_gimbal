/**
 * @file test_whoami.c
 * @author your name (you@domain.com)
 * @brief By default read Who am I register. But can be used for reading and writing to a specific reg. 
 * @version 0.3
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "accel_gyro.h"

/**
 * @brief main app
 * 
 * @param argc number of arguments
 * @param argv[1] reg addres
 * @param argv[2] value to write in reg.
 * @return int (0 - on success; 1 - on failure)
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
