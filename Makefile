MYY_KERNEL_DIR ?= /home/nemanja/rpi-kernel/linux
ARCH ?= arm

obj-m += obj/pwm_driver.o
all:
	arm-linux-gnueabihf-gcc -Wall accel_gyro/src/accel_gyro.c accel_gyro/src/main.c -lm -lrt -std=gnu99 -o bin/program
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD)/pwm_driver -C $(MYY_KERNEL_DIR) modules
	cp pwm_driver/pwm_driver.ko bin/

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) -C $(MYY_KERNEL_DIR) clean
	rm bin/program
