#include <stdio.h>
//#include <time.h>
#include <wiringPi.h>
#define BLACK 25
#define YELLOW 24
#define BROWN 23
#define RED 22

void step(){
  int timp=1500;
 printf("1\n");
  digitalWrite (BLACK, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);

/*
  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);
*/
printf("2\n");

  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, HIGH) ;
 delayMicroseconds(timp);

/*
  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);
*/
printf("3\n");

  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);

/*
  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);
*/
printf("4\n");

  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);

/*
  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);
*/

}

void step2(){
  int timp=10000;

  digitalWrite (BLACK, LOW) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (RED, HIGH) ;
 delayMicroseconds(timp);

  digitalWrite (BLACK, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (RED, LOW) ;
 delayMicroseconds(timp);


  digitalWrite (BLACK, HIGH) ;
  digitalWrite (YELLOW, LOW) ;
  digitalWrite (BROWN, HIGH) ;
  digitalWrite (RED, HIGH) ;
 delayMicroseconds(timp);


  digitalWrite (BLACK, HIGH) ;
  digitalWrite (YELLOW, HIGH) ;
  digitalWrite (BROWN, LOW) ;
  digitalWrite (RED, HIGH) ;
 delayMicroseconds(timp);


}


/*
void counterClockwise(){
int timp=1000;
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

int main (void){
if(wiringPiSetup() == -1){
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


//	for(int i=0;i<512;i++){     // 512 o roatie
while(1){
step();
}  
delay(1000);
//for(int j=0;j<512;j++){
//counterClockwise();
//}

/*
  digitalWrite (PIN37, LOW) ;
  digitalWrite (PIN35, LOW) ;
  digitalWrite (PIN33, LOW) ;
  digitalWrite (PIN31, LOW) ;
  digitalWrite (PIN29, LOW) ;
*/
  return 0 ;
}







