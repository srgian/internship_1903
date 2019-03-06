#include <wiringPi.h>
#include <stdio.h>
#define IN1 21
#define IN2 22

int main(void){
if(wiringPiSetup() == -1){
	printf("setup wiringPi failed! \n");
	return -1;
}
pinMode(IN1, OUTPUT);
pinMode(IN2, OUTPUT);
int i=0;
while(1){
i++;
if(i>=100)
	i=0;
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	printf("OFF\n");
	delay(100-i);
	digitalWrite(IN1, HIGH);
	digitalWrite(IN2, LOW);
	printf("SENS 1\n");
	delay(i);
/*
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, LOW);
	printf("OFF\n");
	delay(5000);
	digitalWrite(IN1, LOW);
	digitalWrite(IN2, HIGH);
	printf("SENS 2\n");
	delay(5000);
*/
}
return 0;
}
