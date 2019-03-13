#include <stdio.h>
//#include <time.h>
#include <wiringPi.h>
#define ORANGE 25
#define YELLOW 24
#define PINK 23
#define BLUE 22
#define RED 21

void Clockwise( int timp){

  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, LOW) ;
  delayMicroseconds(timp);

}

void counterClockwise( int timp){

  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (PINK, HIGH) ;
  digitalWrite (BLUE, HIGH) ;
  delayMicroseconds(timp);

}


int main (void)
{
  int d,v,u,u1;
  if(wiringPiSetup() == -1)
	{
        printf("setup wiringPi failed! \n");
        return -1;
	}
  pinMode (ORANGE, OUTPUT) ;
  pinMode (YELLOW, OUTPUT) ;
  pinMode (PINK, OUTPUT) ;
  pinMode (BLUE, OUTPUT) ;
  pinMode (RED, OUTPUT) ;
  digitalWrite (ORANGE, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (PINK, LOW) ;
  digitalWrite (BLUE, LOW) ;
  digitalWrite (RED, LOW) ;
  printf("Directia?  ( 1 for Clockwise / 2 for CounterClockwise):  \n");
  scanf("%d", &d);
  printf("Viteza? ( >950):  \n");
  scanf("%d", &v);
  printf("Unghiul? \n");
  scanf("%d", &u);
  u1=(360/512)*u;
  float f;
  if (d==1)
    {
      for(f=0.0f;f<u;f+=.703125f)     // 512 o rotatie
      Clockwise(v);
    }
    else if (d==2)for(f=0.0f;f<u;f+=.703125f) counterClockwise(v);
           else printf("1 for Clockwise / 2 for CounterClockwise)");
  
  delay(1000);
/*for(int j=0;j<512;j++){
counterClockwise();
}

  digitalWrite (PIN37, LOW) ;
  digitalWrite (PIN35, LOW) ;
  digitalWrite (PIN33, LOW) ;
  digitalWrite (PIN31, LOW) ;
  digitalWrite (PIN29, LOW) ;
*/
  return 0 ;
}







