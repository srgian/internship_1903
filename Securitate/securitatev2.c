/*****************************************
keypad with finite states
execute with:
gcc alarmas.c -o test -lwiringPi -lpthread
******************************************/

#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "keypad.h"

#define BUTTON 25
#define LEDV 24
#define LEDR 23

enum states
{
    START,
    DEZARMAT,
    ARMAT,
    ALARMA,
    STOP
};

enum states state = START;
int  exit = 0, increment = 0, inc=0, i = 0, press = 0, correct ;
char passlocal[8],passdate[10];
char pass[8],today[10];
pthread_t th_led;
FILE *fp;

void exitp(char x)
{
    if (x == '*')
    {
        state=STOP;
    }
}

void *ledf ()
{
    do
    {
        digitalWrite(LEDR, LOW);
        delay(1000);
        digitalWrite(LEDR, HIGH);
        delay(1000);
    }
    while(correct!=1);
}

void start()
{
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDV, LOW);
    state=DEZARMAT;

}


void buzzArmat()
{
    printf("# Armat! \n");
    digitalWrite(BUZZER, LOW);
    delay(100);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    pass[0] = '\0';
    i = 0;
}
void buzzArmatFail()
{
    printf("Usa deschisa \nSistemul nu a putut fi armat. \n");
    digitalWrite(BUZZER, LOW);
    delay(300);
    digitalWrite(BUZZER, HIGH);
    pass[0] = '\0';
    i = 0;
}
void armare(char x)
{
    if (x == '#')
    {
        inc++;
        if (inc == 20)
        {
            if (digitalRead(BUTTON) != HIGH)
            {
                buzzArmat();
                state = ARMAT;
                inc=0;
                press=1;
            }
            else
            {
                buzzArmatFail();
                state = DEZARMAT;
                inc=0;
                press=1;
            }
        }
    }
    else
    {
        inc=0;
    }
}


void verificare(char x)
{
    if (press == 0)
    {
        press = 1;
        if (x == 'D')
        {
            fp = fopen("parole.txt", "r");
            while (fscanf(fp, " %1023s", passlocal) == 1)
            {
                if (strcmp(passlocal, pass) == 0)
                {
                    correct = 1;
                }
            }
            if (correct == 1)
            {
                printf("Dezarmat\n");
                state = DEZARMAT;
                digitalWrite(BUZZER, LOW);
                delay(100);
                digitalWrite(BUZZER, HIGH);
                pass[0] = '\0';
                i = 0;
                correct = 0;

            }
            else
            {
                /* Create led thread */
                pthread_create( &th_led, NULL, ledf, NULL );
                printf("INCORRECT\n");
                state=ALARMA;
                digitalWrite(BUZZER, LOW);
                delay(300);
                digitalWrite(BUZZER, HIGH);
                pass[0] = '\0';
                i = 0;
            }
        }
        else
        {
            pass[i] = x;
            pass[i + 1] = '\0';
            i++;
            printf("current sequence %s\n", pass);
        }
    }
}

int main(void)
{

    if(wiringPiSetup() == -1)
    {
        printf("setup wiringPi failed! \n");
        return -1;
    }

    pinMode(LEDV, OUTPUT);
    pinMode(LEDR, OUTPUT);

    digitalWrite(LEDV, LOW);

    pinMode(BUTTON, INPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, HIGH);


    init_keypad();
    char x;
    while (exit!=1)
    {

        x = get_key();
        switch(state)
        {
        case START:
            printf("System ON!\nHold # to arm the device.\n");
            start();

            break;
        case DEZARMAT:
            correct=1;

            if(x)
            {
                exitp(x);
                armare(x);
            }
            delay(50);
            break;

        case ARMAT:
            correct=0;

            if(x)
            {
                exitp(x);
                verificare(x);
            }
            else
            {
                press = 0;
                increment = 0;
            }

            delay(50);
            break;
        case ALARMA:

            if(x)
            {
                exitp(x);
                verificare(x);
            }
            else
            {
                press = 0;
                increment = 0;
            }

            delay(50);
            break;
        case STOP:
            exit=1;
            break;
        }
    }
    digitalWrite(LEDV, HIGH);
    digitalWrite(LEDR, HIGH);
    printf("System OFF\n");
    return 0;
}
