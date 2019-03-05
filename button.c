#include <wiringPi.h>
#include <stdio.h>
#define LedPin 21
#define BUTTON 22
int main(void){
if(wiringPiSetup() == -1){
	printf("setup wiringPi failed! \n");
	return -1;
}
pinMode(LedPin, OUTPUT);
pinMode(BUTTON, INPUT);
while(1){ 
	if (digitalRead (BUTTON) !=HIGH)
	{
		digitalWrite(LedPin, HIGH);
		printf("off\n");
	}
	else 
	{
		digitalWrite(LedPin, LOW);
		printf("on\n");
	}
		delay(1000);
	}
return 0;
}
