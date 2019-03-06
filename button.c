#include <wiringPi.h>
#include <stdio.h>
#define LEDPIN 21
#define BUTTON 22
int main(void){
if(wiringPiSetup() == -1){
	printf("setup wiringPi failed! \n");
	return -1;
}
pinMode(LEDPIN, OUTPUT);
pinMode(BUTTON, INPUT);
while(1){ 
	if (digitalRead (BUTTON) !=HIGH)
	{
		digitalWrite(LEDPIN, HIGH);
		printf("off\n");
	}
	else 
	{
		digitalWrite(LEDPIN, LOW);
		printf("on\n");
	}
		delay(1000);
	}
return 0;
}
