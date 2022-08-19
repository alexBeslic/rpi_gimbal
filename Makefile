# The same Makefile without the comments
MYY_KERNEL_DIR ?= /home/nikola/Raspberrypi_tools/rpi-kernel/linux
ARCH ?= arm

obj-m += pwm_driver.o
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) -C $(MYY_KERNEL_DIR) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) -C $(MYY_KERNEL_DIR) clean