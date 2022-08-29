/**
 * @file accel_test.c
 * @author Nemanja
 * @brief Main application for rpi gimble
 * @version 0.1
 * @date 2022-08-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c accel_test.c -lm -lrt -std=gnu99 -o who

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "accel_gyro.h"

static char *servo_file_name = (char*)"/dev/servo_driver";
double beta;
double trsh = 4.0;
int flag = 1;

/**
 * @brief Runs when CTRL+C is prest
 * 
 * @param i not in use
 */
void close_me(int i)
{
	flag = 0;
}

/**
 * @param argc Number of arguments of command line
 * @param argv[1](alfa) The angle around which it balances, default = 0
 * @param argv[2](avarge) The number of angles which is used to find avarge angle, default = 30
 * @param argv[3](ker) Step of incrementing the duration of the pwm signal, default = 4
 * @return 0 
 */
int main(int argc, char **argv)
{
	/* Runs close_me when CTRL+C is prest*/
	signal(SIGINT, close_me);
	float alfa;
	int avarge;
	int ker;
	int kernel = 0;
    double temp;
    uint8_t ret;
    
	switch(argc){
		case 2: 
			alfa = atof(argv[1]);
			break;
		case 3:
			alfa = atof(argv[1]);
			avarge = atoi(argv[2]);
			break;
		case 4:
			alfa = atof(argv[1]);
			avarge = atoi(argv[2]);
			ker = atoi(argv[3]);
			break;
		default:
			alfa = 0;
			avarge = 30;
			ker = 4;		
	}
    
	/* Setup the pwm driver */
    ret = system("sudo insmod pwm_driver.ko");
    
	/* Start angle of servo */
    char buff[10];
    sprintf(buff, "%d", kernel);
	int file_dsc = open(servo_file_name, O_RDWR);
	
	if(file_dsc < 0)
	{
        printf("Driver file can't be opened\n");
        return EXIT_FAILURE;
    }

	write(file_dsc, buff, 10);
	
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
		temp = 0;
		
		for(int i = 0; i < avarge; i++)
		{
			getAngles();
			temp += angles.y;
		}

		printf("Measured angle: %f\n", beta);
		beta = temp / ((double) avarge);
		fflush(stdout);
		
		if((beta - alfa) < (trsh * (-1.0)) || (beta - alfa) > trsh)
		{
			if((int) alfa > (int) beta)
			{
				kernel -= ker;
				sprintf(buff, "%d", kernel);
				write(file_dsc, buff, 10);
			}
			else if((int)alfa < (int)beta)
			{
				kernel += ker;
				sprintf(buff, "%d", kernel);
				write(file_dsc, buff, 10);
			}
		}
    }
    
	close_spi_bus();
    close(file_dsc);
    system("sudo rmmod pwm_driver");
    
    return 0;
}
