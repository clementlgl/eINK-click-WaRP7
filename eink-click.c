void init(void)
{
    unsigned char i;
    CS=1; //perform reset CS idle=1
    SCLK=0; //SCLK idle=0
    RST=1;
    delayms(1);
    RST=0;
    delayms(2);
    RST=1;
    delayms(3);

    writecmd(0x10); //do not enter deep sleep mode
    writedata(0x00);

    writecmd(0x11); //data entry mode setting,0x01,Y decrement,X increment
    writedata(0x01);
    writecmd(0x44); //set RAM X-address start/end position
    writedata(0x00); //RAM X -address start at 00H
    writedata(0x11); //RAM X-address end at 11H->(17D),that is (17+1*4=72)start/end position

    writedata(0xAB); //RAM Y-address start at ABH->(171D)
    writedata(0x00); //RAM Y-address end at 00H
    writecmd(0x4E); //set RAM x address count to 0;
    writedata(0x00);
    writecmd(0x4F); //set RAM Y address count to 172->0;
    writedata(0xAB);

    writecmd(0xF0); //booster feedback selection,0x1F->internal feedback is used
    writedata(0x1F); //0x83
    writecmd(0x21); //bypass the RAM data into the display,enable pass
    writedata(0x03);
    writecmd(0x2C); //write VCOM register
    writedata(0xA0);
    writecmd(0x3C); //board waveform, board voltage
    writedata(0x63);
    writecmd(0x22); //enable sequence, CLK->CP->
    writedata(0xC4);

    writecmd(0x32); //write LUT register
    for(i=0;i<90;i++){
        writedata(lut_data[i]);
    }
}

void fill_display(uchar dat) //0xFF=white, 0x00=black, 0x55=gray 1, 0xAA=gray 2
{
    unsigned int i;
    writecmd(0x24);//data write into RAM after this command

    for(i=0;i<3096;i++) //3096 = 172x72/8x2, (2-Bit per dot)
    {
        writedata(dat);
    }
    writecmd(0x20);

    //Booster diable
    writecmd(0x22); //display updata sequence option ,in page 33
    writedata(0x02);
    writecmd(0x20);
}

