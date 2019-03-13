#include <stdio.h>
#include <wiringPi.h>
#define BLACK 25
#define BROWN 24
#define YELLOW 23
#define RED 22

void Clockwise(){
  int timp=600000;
  digitalWrite (BLACK, HIGH) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (RED, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, HIGH) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (RED, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, LOW) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (RED, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, LOW) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (RED, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (RED, LOW) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (RED, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (RED, HIGH) ;
  delayMicroseconds(timp);

  digitalWrite (BLACK, HIGH) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (RED, HIGH) ;
  delayMicroseconds(timp);

}
/*
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
*/

int main (void)
{
  int d,v,u,u1;
  if(wiringPiSetup() == -1)
	{
        printf("setup wiringPi failed! \n");
        return -1;
	}
  pinMode (BLACK, OUTPUT) ;
  pinMode (YELLOW, OUTPUT) ;
  pinMode (BROWN, OUTPUT) ;
  pinMode (RED, OUTPUT) ;
  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
v=0;
while(v<=1){ Clockwise();v++;} 
/*  printf("Directia?  ( 1 for Clockwise / 2 for CounterClockwise):  \n");
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
/*    else if (d==2)for(f=0.0f;f<u;f+=.703125f) counterClockwise(v);
           else printf("1 for Clockwise / 2 for CounterClockwise)"); */
  delay(1000);
  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;

  return 0 ;
}







