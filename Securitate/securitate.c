/*****************************************
keypad with finite states
execute with:
gcc securitate.c -o securitate -lwiringPi -lpthread
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
#define SNZ 27

enum states
{
    START,
    DEZARMAT,
    ARMAT,
    ALARMA,
    STOP
};

int detectare_miscare=0;
int valoare_senzor;

enum states state = START;
int  exit = 0, button=0, increment = 0, inc=0, i = 0, press = 0, correct ;
pthread_t th_led, th_senzormiscare;
FILE *fp;
char passlocal[9],passdate[10];
char pass[9],azi[10];

void *senzormiscare()
{
    valoare_senzor=digitalRead(SNZ);
    if(valoare_senzor==1){
    	state=ALARMA;
    	printf("miscare detectata.\n");
    	do{
    		//if (state!=ALARMA)
    	//printf("miscare detectata.\n");
        digitalWrite(LEDR,valoare_senzor);
                digitalWrite(LEDR, LOW);
               	delay(1000);
                digitalWrite(LEDR, HIGH);
                delay(1000);
    //state=ALARMA;
    }while(state!=DEZARMAT);
        //detectare_miscare=1;
        //delay(3000);
    }
        digitalWrite(LEDR,HIGH);
        digitalWrite(BUZZER,HIGH);



}

void exitp(char x)
{
    if (x == '*')
    {
        state=STOP;
    }
}

void azii()
{

    int day, month, year;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    day = local->tm_mday;        	// get day of month (1 to 31)
    month = local->tm_mon + 1;   	// get month of year (0 to 11)
    year = local->tm_year + 1900;	// get year since 1900
    sprintf(azi,"%d/%02d/%02d", year, month, day);
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
    button=1;
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
        	if (digitalRead(BUTTON) == HIGH)
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



void verificarebuton()
{
	if (digitalRead(BUTTON) != HIGH)
	{
		button=0;
	}
	else
	{
		button=1;
	}
	if((button==0)&&(state==ARMAT)){
		pthread_create( &th_led, NULL, ledf, NULL );
		printf("Usa deschisa \n");
		state=ALARMA;
	}
}

void verificare(char x)
{
	if (digitalRead(BUTTON) == LOW)
		verificarebuton();
	         /*   {
					if (state == ARMAT){
					printf("Usa deschisa!\n");}
	                state = ALARMA;
	                pthread_create( &th_led, NULL, ledf, NULL );
	            }*/

    if (press == 0)
    {
        press = 1;
        if ((x == 'D')||(i==8))
        {
            fp = fopen("parole.txt", "r");
            while (fscanf(fp, " %1023s", passlocal) == 1)
                        {
                            fscanf(fp, " %1023s", passdate);
                            if (strcmp(passlocal, pass) == 0)
                            {
                                if (strcmp(passdate, azi) >= 0)
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
	azii();
    if(wiringPiSetup() == -1)
    {
        printf("setup wiringPi failed! \n");
        return -1;
    }
    pinMode(SNZ, INPUT);
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
            pthread_create( &th_senzormiscare, NULL, senzormiscare, NULL );
            verificarebuton();
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
        	verificarebuton();
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
