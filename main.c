#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "eink_click.h"

int main(int argc, char * argv[])
{
    int ret;
    
    int spi = open("/dev/spidev0.0", O_RDWR);

    if (spi < 0){
		printf("can't open device");
        exit(EXIT_FAILURE);
    }

    ret = ioctl (spi, SPI_IOC_WR_MODE, SPI_CPHA);
    if(ret < 0){
        printf("Could not set SPIMode (WR)...ioctl fail");
        exit(EXIT_FAILURE);
    }

    ret = ioctl (spi, SPI_IOC_RD_MODE, SPI_CPHA);
    if(ret < 0) {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(EXIT_FAILURE);
    }

    ret = ioctl (spi, SPI_IOC_WR_BITS_PER_WORD, 2);
    if(ret < 0) {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(EXIT_FAILURE);
    }

    ret = ioctl (spi, SPI_IOC_RD_BITS_PER_WORD, 2);
    if(ret < 0) {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(EXIT_FAILURE);
    }

    ret = ioctl (spi, SPI_IOC_WR_MAX_SPEED_HZ, SPI_SPEED);
    if(ret < 0) {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(EXIT_FAILURE);
    }

    ret = ioctl (spi, SPI_IOC_RD_MAX_SPEED_HZ, SPI_SPEED);
    if(ret < 0) {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(EXIT_FAILURE);
    }
    return ret;

    eink_click_init(3);
}

