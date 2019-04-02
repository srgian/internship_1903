#include <wiringPi.h>
#include <stdio.h>
#include <string.h>

#define ROWS 4
#define COLS 4
#define BUZZER 29
#define BUTTON 22


char pressedKey = '\0';
int rowPins[ROWS] = {1, 4, 5, 6};
int colPins[COLS] = {12, 3, 2, 0};

char keys[ROWS][COLS] =
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void init_keypad()
{
    for (int c = 0; c < COLS; c++)
    {
        pinMode(colPins[c], OUTPUT);
        digitalWrite(colPins[c], HIGH);
    }
    for (int r = 0; r < ROWS; r++)
    {
        pinMode(rowPins[0], INPUT);
        pullUpDnControl(rowPins[r], PUD_UP);
    }
}

int findLowRow()
{
    for (int r = 0; r < ROWS; r++)
    {
        if (digitalRead(rowPins[r]) == LOW)
        {
//            printf("Row %d LOW\n", r);
            return r;
        }
        else
        {
//         printf("Row %d HIGH\n", r);
        }
    }
    return -1;
}

char get_key()
{
    int rowIndex;
    for (int c = 0; c < COLS; c++)
    {
        digitalWrite(colPins[c], LOW);
    //    printf("Col %d\n", c);
        // delay(1000);
        rowIndex = findLowRow();
        if (rowIndex > -1)
        {
            if (!pressedKey){ pressedKey = keys[rowIndex][c];digitalWrite(BUZZER, LOW);
                        delay(100);
                        digitalWrite(BUZZER, HIGH);}
            digitalWrite(colPins[c], HIGH);
            return pressedKey;
        }
        digitalWrite(colPins[c], HIGH);
    }
    pressedKey = '\0';
    return pressedKey;
}

int main(void)
{
    int k, correct, increment=0, inc=0, i=0,press=0;
    char test[]="1234";
    char passlocal[2][10]=
    {
        {"1234"},
        {"1111"}
    };
    /*
    FILE *fp;
    char buff[255];
    //fp = fopen("~/internshipr/alarma/parole.txt", "r");
    fp = fopen("parole.txt", "r");
    //    C:\Internshipr\alarma
    fscanf(fp, "%s", buff);
    printf("1: %s\n", buff );

    fgets(buff, 255, (FILE*)fp);
    printf("2: %s\n", buff );

    fgets(buff, 255, (FILE*)fp);
    printf("3: %s\n", buff );
    fclose(fp);

   */

    char pass[100];
    wiringPiSetup();

    pinMode(BUTTON, INPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, HIGH);

    init_keypad();

    while(1)
    {
        char x = get_key();
        if (x)
        {
            if(x=='A')
            {
                increment++;
                if(increment==20)
                {
                    for (int j=0;j<2;j++)
                    {
                        digitalWrite(BUZZER, LOW);
                        delay(100);
                        digitalWrite(BUZZER, HIGH);
                        delay(100);
                    }
                    printf("%d \n",increment);
                }
            }
            if(x=='#')
            {
                inc++;
                if(inc==20)
                {
                    if (digitalRead (BUTTON) !=HIGH)
                	{
                    	  printf("# Armat! \n");
                    	  for (int l=0;l=2;l++)
                          {
                    	    digitalWrite(BUZZER, LOW);
                    	    delay(300);
                    	    digitalWrite(BUZZER, HIGH);
                    	  }
                    	  for (int j=0;j<=5;j++)
                    	  {
                            digitalWrite(BUZZER, LOW);
                            delay(100);
                            digitalWrite(BUZZER, HIGH);
                            delay(1000);
                          }
                        }
                	else printf("Usa deschisa \n");
		       }
		       inc=0;
            }
            if (press==0)
            {
                press=1;
                if(x=='D')
                {
                    for(k=0;k<2;k++)
                    {
                        if(strcmp(passlocal[k], pass)==0)
                    {
                        correct=1;
                    }
                    }
                    if(correct==1)
                    {
                        printf("Dezarmat\n");
                        digitalWrite(BUZZER, LOW);
                        delay(100);
                        digitalWrite(BUZZER, HIGH);
                        pass[0]='\0';
                        i=0;
                        correct=0;
                    }
                    else
                    {
                        printf("INCORRECT\n");
                        digitalWrite(BUZZER, LOW);
                        delay(300);
                        digitalWrite(BUZZER, HIGH);
                        pass[0]='\0';
                        i=0;
                    }
                }
                else
                {
                    pass[i]=x;
                    pass[i+1]='\0';
                    i++;
                    printf("current sequence %s\n", pass);
                }
            }
        }
        else{
            press=0;
            increment=0;
        }

        delay(50);

    }
    return 0;
}
