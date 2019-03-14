#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <time.h>

#define SERVO 1

int main(){
wiringPiSetup();
pinMode(SERVO,OUTPUT);
while(1)
{
	digitalWrite(SERVO,HIGH);
	delayMicroseconds(2000);
	digitalWrite(SERVO,LOW);
	delayMicroseconds(18000);
}
softPwmCreate(SERVO,10,100);
softPwmWrite(SERVO,10);
delay(5000);
/*softPwmWrite(SERVO,13);
delay(5000);
softPwmWrite(SERVO,20);
delay(5000);
return 0;
/*}{
pinMode(0,OUTPUT);
   digitalWrite(1,LOW);
   pwmSetClock(500); 
   softPwmCreate(1,0,500); 
   softPwmWrite(1,14);
return 0;
*/
}

