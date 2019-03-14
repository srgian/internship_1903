#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdint.h>

signed int t_fine;

signed int bmp280_compensate_T_int32 () {

 int fd;
    wiringPiSetup();
    fd = wiringPiI2CSetup(0x76);
    uint8_t fa = wiringPiI2CReadReg8 (fd,0xFA ) ;
    uint8_t fb = wiringPiI2CReadReg8 (fd,0xFB ) ;
    uint8_t fc = wiringPiI2CReadReg8 (fd,0xFC ) ;

    uint8_t x1 = wiringPiI2CReadReg8 (fd,0x88 ) ;
    uint8_t x2 = wiringPiI2CReadReg8 (fd,0x89 ) ;
    uint8_t y1 = wiringPiI2CReadReg8 (fd,0x8A ) ;
    uint8_t y2 = wiringPiI2CReadReg8 (fd,0x8B ) ;
    uint8_t z1 = wiringPiI2CReadReg8 (fd,0x8C ) ;
    uint8_t z2 = wiringPiI2CReadReg8 (fd,0x8D ) ;


int32_t adc=( ( ( fa<<16 ) | ( fb<<8 ) | ( fc<<0 ) ) >> 4 );
int32_t dig_T1=((x2<<8)|(x1<<0));
int32_t dig_T2=((y2<<8)|(y1<<0));
int32_t dig_T3=((z2<<8)|(z1<<0));


    signed int var1, var2, T;
    var1 = ((((adc >> 3) - ((signed int) dig_T1 << 1))) * 
           ((signed int) dig_T2)) >> 11;
    var2 = (((((adc >> 4) - ((signed int) dig_T1)) * 
           ((adc >> 4) - ((signed int) dig_T1))) >> 12) * 
           ((signed int) dig_T3)) >> 14;
    t_fine = var1 + var2;

    T = (t_fine * 5 + 128) >> 8;
    return T;
}


signed int bmp280_compensate_P_int32 () {

 int fd;
    wiringPiSetup();
    fd = wiringPiI2CSetup(0x76);
    uint8_t f7 = wiringPiI2CReadReg8 (fd,0xF7 ) ;
    uint8_t f8 = wiringPiI2CReadReg8 (fd,0xF8 ) ;
    uint8_t f9 = wiringPiI2CReadReg8 (fd,0xF9 ) ;

    uint8_t a1 = wiringPiI2CReadReg8 (fd,0x8E ) ;
    uint8_t a2 = wiringPiI2CReadReg8 (fd,0x8F ) ;
    uint8_t b1 = wiringPiI2CReadReg8 (fd,0x90 ) ;
    uint8_t b2 = wiringPiI2CReadReg8 (fd,0x91 ) ;
    uint8_t c1 = wiringPiI2CReadReg8 (fd,0x92 ) ;
    uint8_t c2 = wiringPiI2CReadReg8 (fd,0x93 ) ;
    uint8_t d1 = wiringPiI2CReadReg8 (fd,0x94 ) ;
    uint8_t d2 = wiringPiI2CReadReg8 (fd,0x95 ) ;
    uint8_t e1 = wiringPiI2CReadReg8 (fd,0x96 ) ;
    uint8_t e2 = wiringPiI2CReadReg8 (fd,0x97 ) ;
    uint8_t f1 = wiringPiI2CReadReg8 (fd,0x98 ) ;
    uint8_t f2 = wiringPiI2CReadReg8 (fd,0x99 ) ;
    uint8_t g1 = wiringPiI2CReadReg8 (fd,0x9A ) ;
    uint8_t g2 = wiringPiI2CReadReg8 (fd,0x9B ) ;
    uint8_t h1 = wiringPiI2CReadReg8 (fd,0x9C ) ;
    uint8_t h2 = wiringPiI2CReadReg8 (fd,0x9D ) ;
    uint8_t i1 = wiringPiI2CReadReg8 (fd,0x9E ) ;
    uint8_t i2 = wiringPiI2CReadReg8 (fd,0x9F ) ;




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


int main ()
{

    long signed int temp = bmp280_compensate_T_int32 ();
     printf("%d.%dC\n", temp/100, temp%100);
    long signed int pres = bmp280_compensate_P_int32 ();
     printf("%d\n",pres);
}
