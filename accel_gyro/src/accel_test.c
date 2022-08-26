#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "accel_gyro.h"
static char *servo_file_name = (char*)"/dev/servo_driver";
/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c accel_test_.c -lm -o who
*/

double beta;
double trsh = 4.0;
int flag = 1;
//sem_t sem_senzor;

/*void* senzor(void* arg)
{
	while(1){
		sem_wait(&sem_senzor);
		float pom = 0;
		for(int i = 0; i < 30; i++){
			getAngles();
			pom += angles.y;
		}
		beta = pom/30.00;
		printf("Beta: %f\n", beta);
		fflush(stdout);
		sem_post(&sem_senzor);
		usleep(1000);
	}
    return 0;
}
*/
void close_me(int i)
{
	flag = 0;
}

int main(int argc, char **argv)
{
	signal(SIGINT, close_me);
	
    uint8_t ret;
    //pthread_t p_senzor;
    float alfa = atof(argv[1]);
    int srednje = atoi(argv[2]);
    int ker = atoi(argv[3]);
    int kernel = 0;
    double pom;
    //sem_init(&sem_senzor, 0, 0);
    
    ret = system("sudo insmod pwm_driver.ko");
    
    char buff[10];
    sprintf(buff, "%d", kernel);
	int file_dsc = open(servo_file_name, O_RDWR);
	
	if(file_dsc < 0)
	{
        printf("Fajl se nije otvorio\n");
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

    /*ret = calculate_error();

    if (ret != 0)
    {
        printf("Error occurred while configuring accel\n");
        return EXIT_FAILURE;
    }*/
	//sem_post(&sem_senzor);
    //pthread_create(&p_senzor, NULL, senzor, NULL);
    sleep(3);
    while (flag)
    {
		//sem_wait(&sem_senzor);
		
		pom = 0;
		for(int i = 0; i < srednje; i++){
			getAngles();
			pom += angles.y;
		}
		printf("pom: %f agels: %f beta: %f\n", pom, angles.y, beta);
		beta = pom/((double) srednje);
		//printf("Beta: %f\n", beta);
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
		
		//sem_post(&sem_senzor);
		//usleep(10);
    }
    
    close(file_dsc);
    system("sudo rmmod pwm_driver");
    
    return 0;
}
