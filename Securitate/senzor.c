#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define LEDR 7
#define LEDV 28
#define SNZ 27
#define BUZZ 26

int detectare_miscare=0;
int valoare_senzor;

void setup()
{
    pinMode(LEDR, OUTPUT);
    pinMode(LEDV, OUTPUT);
    pinMode(SNZ, INPUT);
    pinMode(BUZZ, OUTPUT);

    digitalWrite(BUZZ, HIGH);
    digitalWrite(LEDV,LOW);
    digitalWrite(LEDR,LOW);
    delay(60000);  //doar la punerea in functiune, senzorul se aclimatizeaza la energia infrarosie din incapere; 
                   //linia se poate comenta dupa aceea 
    digitalWrite(LEDV,HIGH);
}

void loop()
{
    valoare_senzor=digitalRead(SNZ);
    if(valoare_senzor==1){
        digitalWrite(LEDR,valoare_senzor);
        digitalWrite(BUZZ,LOW);
        detectare_miscare=1;
        delay(3000); //led-ul rosu ramane aprins 3 secunde, iar buzzerul la fel
    } else {
        digitalWrite(LEDR,LOW);
        digitalWrite(BUZZ,HIGH);
    }

    if(detectare_miscare==1){   //resetare la valorile initiale
        digitalWrite(BUZZ, HIGH);
        digitalWrite(LEDV,LOW);
        digitalWrite(LEDR,LOW);
        delay(6000);    //6 secunde pana va putea detecta din nou miscare
        digitalWrite(LEDV,HIGH);
        detectare_miscare=0;
    }
}
int main()
{
     if(wiringPiSetup() == -1){
        printf("setup wiringPi failed! \n");
        return -1;
    }
    setup();
    for(;;)
        loop();
    return 0;
}
