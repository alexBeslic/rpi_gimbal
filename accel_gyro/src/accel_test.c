#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "accel_gyro.h"
static char *servo_file_name = (char*)"/dev/servo_driver";

double beta;
double trsh = 4.0;
int flag = 1;

void close_me(int i)
{
	flag = 0;
}

int main(int argc, char **argv)
{
	/* Runs close_me when CTRL+C is prest*/
	signal(SIGINT, close_me);
	float alfa;
	int srednje;
	int ker;
	int kernel = 0;
    double pom;
    uint8_t ret;
    
	switch(argc){
		case 2: 
			alfa = atof(argv[1]);
			break;
		case 3:
			alfa = atof(argv[1]);
			srednje = atoi(argv[2]);
			break;
		case 4:
			alfa = atof(argv[1]);
			srednje = atoi(argv[2]);
			ker = atoi(argv[3]);
			break;
		default:
			alfa = 0;
			srednje = 30;
			ker = 4;		
	}
    
	/* Setup the pwm driver*/
    ret = system("sudo insmod pwm_driver.ko");
    
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
		pom = 0;
		
		for(int i = 0; i < srednje; i++)
		{
			getAngles();
			pom += angles.y;
		}

		printf("Izmjereni ugao: %f\n", beta);
		beta = pom / ((double) srednje);
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
		//usleep(1000);
    }
    
	close_spi_bus();
    close(file_dsc);
    system("sudo rmmod pwm_driver");
    
    return 0;
}
