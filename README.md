# rpi_gimbal
Single axis gimbal using Raspberry Pi 2 model B, 6DOF IMU 5 CLICK(ICM-20789) and servo motor BS-422.  

## Enabling The SPI Port on the Raspberry Pi
Use the raspbian config tool: 

    sudo raspi-config  
    
Select 'Interfacing Options' and then select the relevant option.   
After rebooting your RPI check if everything is working run:

    ls -l /dev/spidev*

For more info see [here](https://pimylifeup.com/raspberry-pi-spi/)

## Enabling hardware PWM on the Raspberry Pi
Edit the /boot/config.txt file and add the following line to use GPIO_18 as the pin for PWM0:

    dtoverlay=pwm,pin=18,func=2

After rebooting your RPI check if everything is working run:

    lsmod | grep pwm 

and looking for **pwm_bcm2835**  
See: [here](https://github.com/dotnet/iot/blob/main/Documentation/raspi-pwm.md#:~:text=IOException%3A%20Permission%20denied-,Enabling%20hardware%20PWM,-In%20order%20to)

## Wire conections
For the servo motor you need to provide +5 V, GND and PWM signal wire, by default it's GPIO_18.
For the 6DOF IMU 5 CLICK(ICM-20789) you need to provide +3.3 V, GND, SDI(GPIO_09 on RPI), SDO(GPIO_10 on RPI), SCK(GPIO_11 on RPI), CS(GPIO_08 on RPI)


## Compile and run
Compiling is set up as cross-compile for Raspberry Pi OS to work in real-time.   

#### Main app
To compile the main app and the PWM driver run the included Makefile (note: path to linux source code maybe different on our PC).  
It will generate **pwm_driver.ko** and **program** in the **bin** directory that you need to copy to your RPI. The main app will insert module and stabilize Y axis to 0 degrees. You can provide diffrent arguments to the main app. If you want to stabilize Y axis to a diffrent angle you can run:

    sudo ./program 15


If you want to incres the number of angles which is used to find avrage angle (default it's 30) and step of increment duration of the pwm signal (default it's 4) run:

    sudo ./program 0 40 5


If you want to use the sensor to display the accel and gyro data. And the appropriate angles. First compile the code with the following line:

    arm-linux-gnueabihf-gcc -Wall accel_gyro.c accel_angles.c -lm -lrt -std=gnu99 -o angles

It will generate **angles** file that you need to copy to your RPI and run it.

#### PWM driver
To compile the RPI PWM driver for servo motor run the includet Makefile (note: path to linux source code maybe different on our PC). After running the Makefile  pwm_driver.ko will be generated. You need to copy that file to your RPI. To insert module run:

    sudo insmod pwm_driver.ko

To remove the module run:

    sudo rmmod pwm_driver

You can use the module raw just write from to 0 to 400 and it is mapped to it's min and max position. Example:

    echo "69" >> /dev/servo_driver

note: you need to be sudo su.  
