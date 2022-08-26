#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "accel_gyro.h"
static char *servo_file_name = (char*)"/dev/servo_driver";
/*
    compile:
    
    arm-linux-gnueabihf-gcc -Wall accel_gyro.c accel_test_.c -lm -o who
*/

float beta;
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
float pid_p,pid_i, pid_d;
float p=4.0,i=0.1,d=0.1;
float error, previous_error;
float pid_u;

long long p_time,c_time,e_time;


int main(int argc, char **argv)
{
    uint8_t ret;
    //pthread_t p_senzor;
    float alfa = atof(argv[1]);
    int srednje = atoi(argv[2]);
    int kernel = 0;
    //sem_init(&sem_senzor, 0, 0);
    
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
    while (1)
    {
		float pom = 0;
		for(int i = 0; i < srednje; i++){
			getAngles();
			pom += angles.y;
		}
		printf("pom: %f\n", pom);
		beta = pom/((float) srednje);
		//printf("Beta: %f\n", beta);
		fflush(stdout);
		
		//sem_wait(&sem_senzor);
		p_time = c_time;
		c_time = current_time();
		e_time = (c_time - p_time)/1000;
		
		error=beta-alfa;
		pid_p=p*error;
		pid_d=d*(error-previous_error)/e_time;
		
		if(-3<error && error< 3){
				pid_i=pid_i+(i*error);
		}
		
		pid_u=pid_i+pid_d+pid_p;
		
		previous_error = error;
		
		
		
		
		sprintf(buff, "%d", (int)pid_u + 90);
		write(file_dsc, buff, 10);
		
		//sem_post(&sem_senzor);
		//usleep(10);
    }
    
}
