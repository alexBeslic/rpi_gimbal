#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "accel_gyro.h"
/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c accel_test_.c -lm -o who
*/

float beta;

void* senzor(void* arg)
{
	while(1){
		float pom = 0;
		for(int i = 0; i < 300; i++){
			getAngles();
			pom += angles.y;
		}
		beta = pom/300.00;
		printf("Beta: %f\n", beta);
		fflush(stdout);
	}
    return 0;
}

int main(int argc, char **argv)
{
    uint8_t ret;
    pthread_t p_senzor;
	
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

    pthread_create(&p_senzor, NULL, senzor, NULL);
    sleep(3);
    pthread_join(p_senzor, NULL);
}
