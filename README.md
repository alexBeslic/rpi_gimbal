# rpi_gimbal
Single axis gimbal using Raspberry Pi and Accel 13 Click.

## Enabling The I2C Port
Use the raspbian config tool: 

    sudo raspi-config  
    
Select 'Advanced Options' and then select the relevant option.  

Install the I2C tools:  

    sudo apt-get install i2c-tools  
    sudo apt-get update 
    
At the command prompt type one of these depending on whether you are using the I2C0 or I2C1 port:
 
    sudo i2cdetect -y 0  
    //or  
    sudo i2cdetect -y 1

Add the user 'pi' to I2C users so that you don't have to run the I2C tools as root:

    sudo adduser pi i2c

Setting the I²C Bus Speed:  
Edit the /boot/config.txt file and add the following line for 400 kHz bus speeds

    dtparam=i2c_baudrate=400000

References:
[link-1](https://raspberry-projects.com/pi/programming-in-c/i2c/using-the-i2c-interface)
[link-2](https://www.hobbytronics.co.uk/raspberry-pi-raspbian-distro#:~:text=cd%20..-,I2C%20Tools%20Package,-There%20is%20a)
[link-3](https://www.abelectronics.co.uk/kb/article/1/i2c-part-2---enabling-i-c-on-the-raspberry-pi)
## Enabling hardware PWM
Edit the /boot/config.txt file and add the following line to use GPIO_18 as the pin for PWM0:

    dtoverlay=pwm,pin=18,func=2

After rebooting your RPI check if everything is working run:

    lsmod | grep pwm 

and looking for **pwm_bcm2835**  
See: [here](https://github.com/dotnet/iot/blob/main/Documentation/raspi-pwm.md#:~:text=IOException%3A%20Permission%20denied-,Enabling%20hardware%20PWM,-In%20order%20to)
