//execute with: sudo g++ -I . I2Cdev.cpp MPU6050.cpp demo_raw.cpp -o demo_raw2 -lwiringPi

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <wiringPi.h>
#include <stdlib.h>
#include "I2Cdev.h"
#include "MPU6050.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az, ax1, ay1, az1;
int16_t gx, gy, gz, gx1, gy1, gz1;

void setup() {
    // initialize device
    printf("Initializing I2C devices...\n");
    accelgyro.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");
}

void gotoxy(int x,int y)
{
printf("%c[%d;%df",0x1B,y,x);
}

void inclinarex(int x){
	int grade = x/180;
        gotoxy(174,13);
	printf("%d grade",grade);
	switch(grade){
	case -15 ... 15: // 0 grade
		gotoxy(130,13);
		printf(" _______________________________________");
break;
	case 16 ... 37: // 30 grade
	       for (int i=1;i<40;i++){
                if(i%5==0){
                	gotoxy(130+i,18-(i/5));
                	printf("/");}
                else{
                	gotoxy(130+i,17-(i/5));
                	printf("_");}
          }
break;
	case 38 ... 53: // 45 grade
	for (int i=1;i<30;i++){
                if(i%2==0){
                	gotoxy(134+i,21-(i/2));
                	printf("/");}
                else{
                	gotoxy(134+i,20-(i/2));
                	printf("_");}

}

break;

	case 54 ...75: // 60 grade
	for(int i=1;i<18;i++){
                gotoxy(140+i,22-i);
                printf("/");}

break;

	case 76 ... 100: // 90 grade
        for(int i=1;i<21;i++){
                gotoxy(150,24-i);
                printf("|");}
break;
	case -37 ... -16: // -30 grade
               for (int i=1;i<40;i++){
                if(i%5==0){
                        gotoxy(130+i,9+(i/5));
                        printf("\\");}
                else{
                        gotoxy(130+i,9+(i/5));
                        printf("_");}
}break;
	case  -53 ... -38: // -45 grade
	        for (int i=1;i<30;i++){
                if(i%2==0){
                        gotoxy(135+i,6+(i/2));
                        printf("\\");}
                else{
                        gotoxy(135+i,6+(i/2));
                        printf("_");}
}break;
	case -75 ... -54: // -60 grade
        	for(int i=1;i<20;i++){
                	gotoxy(140+i,3+i);
                	printf("\\");}
break;
	case -100 ... -76: // -90 grade
		for(int i=1;i<21;i++){
                	gotoxy(150,3+i);
                	printf("|");}
break;



	default:
		gotoxy(150,12);
		printf("erroare");
}
}

void accx(int x){
	if (x<0) //verificam daca x este pozitiv sau negativ sa miscam la stanga sau la dreapta nivelul
	  {
	   int xx= abs(x);

            int xstep=xx/1300; //un character apare la fiecare 1300 de unitati
            gotoxy(120,20); // pozitia
            printf("|");
            for(int i=1;i<=xstep;++i) //desenam o linie pentru fiecare unitate
	      {
                gotoxy(120-i,20);
                printf("-");

	      }
	  }
	else
	  {
            int xstep= x/1300;
	    gotoxy(120,20);
	    printf("|");
            for(int i=1;i<=xstep;++i)
	      {
                gotoxy(120+i,20);
                printf("-");
	      }
	  }
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
system("clear");

    // display accel/gyro x/y/z values
    gotoxy(5,12);
    printf("a/g: %6hd %6hd %6hd   %6hd %6hd %6hd\n",ax,ay,az,gx,gy,gz);
    gotoxy(94,24);
    printf("| Axa X - Inclinatia si acceleratia");
    gotoxy(93,24);
    printf("Valori");
    gotoxy(100,26);
    printf("Axa Z");
    gotoxy(94,26);
    printf("Axa Y");


   accx(gx);
   inclinarex(ax);
}


int main()
{
    system("clear");
    setup();
    delayMicroseconds(100000);
    system("clear");
    for (;;)
      {
        loop();
        delayMicroseconds(100000);
      }
}

