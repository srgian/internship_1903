#include <wiringPi.h>
#include <softPwm.h>

#define GPIO1 0
//#define GPIO2 7

int main(int argc, char *argv[])
{
   if (wiringPiSetupGpio() < 0) return 1;

   pinMode(GPIO1, OUTPUT);
   digitalWrite(GPIO1, LOW);
   softPwmCreate(GPIO1, 0, 500);
   softPwmWrite(GPIO1, 15);
/*
   pinMode(GPIO2, OUTPUT);
   digitalWrite(GPIO2, LOW);
   softPwmCreate(GPIO2, 0, 500);
   softPwmWrite(GPIO2, 15);
*/
//   sleep(30);
delay(5000);
}
