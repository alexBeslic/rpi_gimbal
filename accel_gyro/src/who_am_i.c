#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

uint8_t mode = 0;
uint32_t speed = 8000000; // 8 MHz
uint8_t bits = 8;

void spi_transfer(int fd,uint8_t *data, int length){
    struct spi_ioc_transfer spi[length];

    for(int i=0;i<length;i++){
        memset(&spi[i],0,sizeof(struct spi_ioc_transfer));
        spi[i].tx_buf=(unsigned long)(data+i);
        spi[i].rx_buf=(unsigned long)(data+i);
        spi[i].len=1;
        spi[i].speed_hz=speed;
        spi[i].bits_per_word=bits;
    }
    if(ioctl(fd,SPI_IOC_MESSAGE(length),spi)<0){
        perror("Cetic");
        close(fd);
        exit(-1);
    }
}


int main()
{
    int fd;
    uint8_t data[2];

    fd = open("/dev/spidev0.0", O_RDWR);
    if (fd < 0)
    {
        perror("Error opening SPI Bus");
        return -1;
    }

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        perror("Error setting SPI mode");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        perror("Error setting SPI speed");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
    {
        perror("Error setting SPI wordlength");
        close(fd);
        return -1;
    }

    data[0]=0x80 | 0x75;
    data[1]=0x75;
    spi_transfer(fd,data,2);

    printf("%x\n%x\n",data[0],data[1]);

    close(fd);
    return 0;
}
