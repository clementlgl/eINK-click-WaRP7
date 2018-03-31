#include "eink-click.h"

void init(int dev)
{
    unsigned char i;
    /*CS=1; //perform reset CS idle=1
    SCLK=0; //SCLK idle=0
    RST=1;
    delayms(1);
    RST=0;
    delayms(2);
    RST=1;
    delayms(3);*/

    spi_transfer(dev, 0x10); //do not enter deep sleep mode
    spi_transfer(dev, 0x00);

    spi_transfer(dev, 0x11); //data entry mode setting,0x01,Y decrement,X increment
    spi_transfer(dev, 0x01);
    spi_transfer(dev, 0x44); //set RAM X-address start/end position
    spi_transfer(dev, 0x00); //RAM X -address start at 00H
    spi_transfer(dev, 0x11); //RAM X-address end at 11H->(17D),that is (17+1*4=72)start/end position

    spi_transfer(dev, 0xAB); //RAM Y-address start at ABH->(171D)
    spi_transfer(dev, 0x00); //RAM Y-address end at 00H
    spi_transfer(dev, 0x4E); //set RAM x address count to 0;
    spi_transfer(dev, 0x00);
    spi_transfer(dev, 0x4F); //set RAM Y address count to 172->0;
    spi_transfer(dev, 0xAB);

    spi_transfer(dev, 0xF0); //booster feedback selection,0x1F->internal feedback is used
    spi_transfer(dev, 0x1F); //0x83
    spi_transfer(dev, 0x21); //bypass the RAM data into the display,enable pass
    spi_transfer(dev, 0x03);
    spi_transfer(dev, 0x2C); //write VCOM register
    spi_transfer(dev, 0xA0);
    spi_transfer(dev, 0x3C); //board waveform, board voltage
    spi_transfer(dev, 0x63);
    spi_transfer(dev, 0x22); //enable sequence, CLK->CP->
    spi_transfer(dev, 0xC4);

    writecmd(0x32); //write LUT register
    for(i=0;i<90;i++){
        spi_transfer(dev, lut_data[i]);
    }
}

void fill_display(int dev, char data) //0xFF=white, 0x00=black, 0x55=gray 1, 0xAA=gray 2
{
    unsigned int i;
    spi_transfer(dev, 0x24);//data write into RAM after this command

    for(i=0;i<3096;i++) //3096 = 172x72/8x2, (2-Bit per dot)
    {
        spi_transfer(dev, data);
    }
    spi_transfer(dev, 0x20);

    //Booster diable
    spi_transfer(dev, 0x22); //display updata sequence option ,in page 33
    spi_transfer(dev, 0x02);
    spi_transfer(dev,0x20);
}

int spi_transfer(int dev, int *tx, int *rx = NULL){

    struct spi_ioc_transfer tr;
    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = sizeof(tx);
    tr.delay_usecs = DElAY_USEDCS;
    tr.speed_hz = SPI_SPEED;
    tr.bits_per_word = 2;

    int ret = ioctl(dev, SPI_IOC_MESSAGE(1), &tr);

    return ret;
}