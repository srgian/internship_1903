#include <wiringPi.h>
#include <stdio.h>
#define LedPin 21

int main(void){
if(wiringPiSetup() == -1){
	printf("setup wiringPi failed! \n");
	return -1;
}
pinMode(LedPin, OUTPUT);
while(1){
	digitalWrite(LedPin, LOW);
	printf("on\n");
	delay(1000);
	digitalWrite(LedPin, HIGH);
	printf("off\n");
	delay(1000);
}
return 0;
}
