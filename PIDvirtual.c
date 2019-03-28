    #include <wiringPiI2C.h>
    #include <wiringPi.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <dirent.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <string.h>
    #include <softPwm.h>
    #include <time.h>
    #define I2C_ADDR   0x27 // I2C device address
    #define LCD_CHR  1 // Mode - Sending data
    #define LCD_CMD  0 // Mode - Sending command
    #define LINE1  0x80 // 1st line
    #define LINE2  0xC0 // 2nd line
    #define LCD_BACKLIGHT   0x08  // On
    #define ENABLE  0b00000100 // Enable bit
    #define YELLOW 23

    void lcd_init(void);
    void lcd_byte(int bits, int mode);
    void lcd_toggle_enable(int bits);

    // added by Lewis
    void typeInt(int i);
    void typeFloat(float myFloat);
    void lcdLoc(int line); //move cursor
    void ClrLcd(void); // clr LCD return home
    void typeln(const char *s);
    void typeChar(char val);
    signed int bmp280_compensate_P_int32 ();
    signed int bmp280_compensate_T_int32 ();
    uint8_t getOneWire(float * temp);
    int fd;  // seen by all subroutines
    int fds;
    signed int t_fine;
    float tempC;

    int main()
    {
        wiringPiSetup();
        pinMode(YELLOW,OUTPUT);
        if (wiringPiSetup () == -1) exit (1);
        fd = wiringPiI2CSetup(I2C_ADDR);
        fds = wiringPiI2CSetup(0x77);
        lcd_init();
        softPwmCreate(YELLOW,0,200);

        long signed int temp=0;
        long signed int pres=0;

        float T_setat;
        float T_citit=0;
        float Eror=0;
        float ErorI=0;
        float ErorD=0;
        float prima_eroare=0;
        float Kp=10;
        float Ki=2;
        float Kd=1;
        float Mc;

        printf("Introduceti temperatura care se doreste a fi setata \n");
        scanf("%f", &T_setat);
        while(1)
        {
            temp = bmp280_compensate_T_int32 ();
            pres = bmp280_compensate_P_int32 ();
            lcdLoc(LINE1);
            typeln("Temp:");
            typeFloat(temp/100.0f);
            delay(100);
            if(getOneWire(&tempC))
            {
                lcdLoc(LINE2);
                typeln("tempC:");
                typeFloat(tempC/1000.0f);
            }
            delay(100);
            T_citit=tempC/1000.0f;
            Eror= T_setat - T_citit;
            ErorI=ErorI + Eror;
            ErorD=Eror - prima_eroare;
            prima_eroare = Eror;
            Mc=Kp*Eror+Ki*ErorI+Kd*ErorD;
            if (Mc>199)   {Mc=199;}
            if (Mc<1)   {Mc=1;}
            if (ErorI>100)   {ErorI=100;}
            if (ErorI<-100)   {ErorI=-100;}
            printf ("T_citit: %3.2f T_setat: %3.2f Eror: %3.2f Pri.er: %3.2f EroareI: %3.2f EroareD.: %3.2f Kp= %3.2f Ki= %3.2f Kd= %3.2f Mc= %3.2f \n"
                    , T_citit,T_setat,Eror,prima_eroare,ErorI,ErorD,Kp,Ki,Kd,Mc);
            int Com = (int) Mc;
            softPwmWrite(YELLOW,Com);
            delay (1000);
     }
}


// float to string
void typeFloat(float myFloat)   {
  char buffer[20];
  sprintf(buffer, "%4.2f",  myFloat);
  typeln(buffer);
}

// int to string
void typeInt(int i)   {
  char array1[20];
  sprintf(array1, "%d",  i);
  typeln(array1);
}

// clr lcd go home loc 0x80
void ClrLcd(void)   {
  lcd_byte(0x01, LCD_CMD);
  lcd_byte(0x02, LCD_CMD);
}

// go to location on LCD
void lcdLoc(int line)   {
  lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void typeChar(char val)   {

  lcd_byte(val, LCD_CHR);
}


// this allows use of any size string
void typeln(const char *s)   {

  while ( *s ) lcd_byte(*(s++), LCD_CHR);

}

void lcd_byte(int bits, int mode)   {

  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void lcd_toggle_enable(int bits)   {
  // Toggle enable pin on LCD display
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits | ENABLE));
  delayMicroseconds(500);
  wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
  delayMicroseconds(500);
}


void lcd_init()   {
  // Initialise display
  lcd_byte(0x33, LCD_CMD); // Initialise
  lcd_byte(0x32, LCD_CMD); // Initialise
  lcd_byte(0x06, LCD_CMD); // Cursor move direction
  lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
  lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
  lcd_byte(0x01, LCD_CMD); // Clear display
  delayMicroseconds(500);
}

signed int bmp280_compensate_T_int32 () {
    uint8_t fa = wiringPiI2CReadReg8 (fds,0xFA ) ;
    uint8_t fb = wiringPiI2CReadReg8 (fds,0xFB ) ;
    uint8_t fc = wiringPiI2CReadReg8 (fds,0xFC ) ;
    uint8_t x1 = wiringPiI2CReadReg8 (fds,0x88 ) ;
    uint8_t x2 = wiringPiI2CReadReg8 (fds,0x89 ) ;
    uint8_t y1 = wiringPiI2CReadReg8 (fds,0x8A ) ;
    uint8_t y2 = wiringPiI2CReadReg8 (fds,0x8B ) ;
    uint8_t z1 = wiringPiI2CReadReg8 (fds,0x8C ) ;
    uint8_t z2 = wiringPiI2CReadReg8 (fds,0x8D ) ;

    int32_t adc=( ( ( fa<<16 ) | ( fb<<8 ) | ( fc<<0 ) ) >> 4 );
    int32_t dig_T1=((x2<<8)|(x1<<0));
    int32_t dig_T2=((y2<<8)|(y1<<0));
    int32_t dig_T3=((z2<<8)|(z1<<0));

    int32_t  var1, var2, T;

    var1 = ((((adc >> 3) - ((int32_t) dig_T1 << 1))) *
           ((int32_t) dig_T2)) >> 11;

    var2 = (((((adc >> 4) - ((int32_t) dig_T1)) *
           ((adc >> 4) - ((int32_t) dig_T1))) >> 12) *
           ((int32_t) dig_T3)) >> 14;

    t_fine = var1 + var2;

    T = (t_fine * 5 + 128) >> 8;

    return T;
}

signed int bmp280_compensate_P_int32 () {
    uint8_t f7 = wiringPiI2CReadReg8 (fds,0xF7 ) ;
    uint8_t f8 = wiringPiI2CReadReg8 (fds,0xF8 ) ;
    uint8_t f9 = wiringPiI2CReadReg8 (fds,0xF9 ) ;

    uint8_t a1 = wiringPiI2CReadReg8 (fds,0x8E ) ;
    uint8_t a2 = wiringPiI2CReadReg8 (fds,0x8F ) ;
    uint8_t b1 = wiringPiI2CReadReg8 (fds,0x90 ) ;
    uint8_t b2 = wiringPiI2CReadReg8 (fds,0x91 ) ;
    uint8_t c1 = wiringPiI2CReadReg8 (fds,0x92 ) ;
    uint8_t c2 = wiringPiI2CReadReg8 (fds,0x93 ) ;
    uint8_t d1 = wiringPiI2CReadReg8 (fds,0x94 ) ;
    uint8_t d2 = wiringPiI2CReadReg8 (fds,0x95 ) ;
    uint8_t e1 = wiringPiI2CReadReg8 (fds,0x96 ) ;
    uint8_t e2 = wiringPiI2CReadReg8 (fds,0x97 ) ;
    uint8_t f1 = wiringPiI2CReadReg8 (fds,0x98 ) ;
    uint8_t f2 = wiringPiI2CReadReg8 (fds,0x99 ) ;
    uint8_t g1 = wiringPiI2CReadReg8 (fds,0x9A ) ;
    uint8_t g2 = wiringPiI2CReadReg8 (fds,0x9B ) ;
    uint8_t h1 = wiringPiI2CReadReg8 (fds,0x9C ) ;
    uint8_t h2 = wiringPiI2CReadReg8 (fds,0x9D ) ;

    uint8_t i1 = wiringPiI2CReadReg8 (fds,0x9E ) ;
    uint8_t i2 = wiringPiI2CReadReg8 (fds,0x9F ) ;
    int32_t adc=( ( ( f7<<16 ) | ( f8<<8 ) | ( f9<<0 ) ) >> 4 );
    int32_t dig_P1=((a2<<8)|(a1<<0));
    int32_t dig_P2=((b2<<8)|(b1<<0));
    int32_t dig_P3=((c2<<8)|(c1<<0));
    int32_t dig_P4=((d2<<8)|(d1<<0));

    int32_t dig_P5=((e2<<8)|(e1<<0));
    int32_t dig_P6=((f2<<8)|(f1<<0));
    int32_t dig_P7=((g2<<8)|(g1<<0));
    int32_t dig_P8=((h2<<8)|(h1<<0));
    int32_t dig_P9=((i2<<8)|(i1<<0));
    int64_t var1, var2, p;

    var1 = (int64_t)t_fine-128000;
    var2 = var1 * var1 *(int64_t) dig_P6;
    var2 = var2 + ((var1 *(int64_t) dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4)<<35);
    var1 = ((var1 * var1 *(int64_t)dig_P3)>>8) + ((var1 *(int64_t)dig_P2)<<12);
    var1 = ((((int64_t)1)<<47) + var1) * (((int64_t)dig_P1) >> 33);
    if (var1 == 0)
    {
        return 0;
    }

    p= 1048576-adc;
    p= (((p<<31)-var2)*3125)/var1;
    var1 = (((int64_t)dig_P9) *( p>>13)*(p>>13))>>25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p+var1+var2)>>8) + (((int64_t)dig_P7)<<4);

    return(uint32_t) p;
}

uint8_t getOneWire(float * temp)
{
    DIR *dir;
    struct dirent *dirent;
    char dev[16];      // Dev ID
    char devPath[128]; // Path to device
    char buf[256];     // Data from device
    char tmpData[6];   // Temp C * 1000 reported by device
    char path[] = "/sys/bus/w1/devices";
    ssize_t numRead;

    dir = opendir (path);
    if (dir != NULL)
    {
        while ((dirent = readdir (dir)))
        // 1-wire devices are links beginning with 28-
        if (dirent->d_type == DT_LNK && strstr(dirent->d_name, "28-") != NULL)
        {
            strcpy(dev, dirent->d_name);
            //printf("\nDevice: %s\n", dev);
        }
        (void) closedir (dir);
    }
    else
    {
        perror ("Couldn't open the w1 devices directory");
        return 0;
    }

    sprintf(devPath, "%s/%s/w1_slave", path, dev);

    while(1) {

       // printf("%s\n", devPath);
        int fd = open(devPath, O_RDONLY);
        if(fd == -1)
        {
            perror ("Couldn't open the w1 device.");
            return 0;
        }
        while((numRead = read(fd, buf, 256)) > 0)
        {
            strncpy(tmpData, strstr(buf, "t=") + 2, 5);
            float tempC = strtof(tmpData, NULL);
            *temp=tempC;
           // printf("Device: %s  - ", dev);
            //printf("Temp: %.3f C  ", tempC / 1000);
           // printf("%.3f F\n\n", (tempC / 1000) * 9 / 5 + 32);
            return 1;
        }
        close(fd);
    }

}
