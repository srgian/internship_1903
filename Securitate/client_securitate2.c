/*****************************************
execute with:
gcc securitate.c -o securitate -lwiringPi -lpthread
******************************************/

#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <postgresql/libpq-fe.h>

#include "keypad.h" //fisier header cu functiile pentru keypad

#define BUTTON 25
#define LEDV 24
#define LEDR 23
#define SNZ 27

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

enum states
{
    START,
    DEZARMAT,
    PINV,
    PINI,
    ARMAT,
    ALARMA,
    STOP
};

PGconn          *conn;
PGresult        *res;


char sql[255];


int detectare_miscare=0;
int valoare_senzor;
int  exitv = 0, button=0, increment = 0, inc=0, i = 0, j = 0, press = 0, correct, sockfd ;
char passlocal[9],passdate[10],admin[]="9999";
char pass[9],azi[10];
int dd=0, mm=0, yy=0;
char dataintrodusa[12],datacurenta[12];
char date[22];
int day, month, year;
pthread_t th_led, th_senzormiscare, th_clientsend;
FILE *fp;
char buff[MAX];
char s[10]="Pornit";

enum states state = START;



//   THREADS

void *senzormiscare() // thread verificare miscare
{
    valoare_senzor=digitalRead(SNZ);
    if(valoare_senzor==1)
    {
    	databasealarma();
    	state=ALARMA;
        printf("miscare detectata.\n");
        do
        {
            digitalWrite(LEDR,valoare_senzor);
            digitalWrite(LEDR, LOW);
            delay(1000);
            digitalWrite(LEDR, HIGH);
            delay(1000);
        }
        while(state!=DEZARMAT);
    }
    digitalWrite(LEDR,HIGH);
    digitalWrite(BUZZER,HIGH);
    pthread_exit(NULL);
}

void *ledf () //thread functionare leduri in starea de alarma
{
    do
    {
        digitalWrite(LEDR, LOW);
        delay(1000);
        digitalWrite(LEDR, HIGH);
        delay(1000);
    }
    while(correct!=1);
    pthread_exit(NULL);
}

void *clientsend () //thread trimitere date client
{

    do
    {
        bzero(buff, sizeof(buff));
        sprintf(buff,"%s", s);
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        delay(1000);
    }
    while(state!=STOP);
    sprintf(s,"Oprire");
    sprintf(buff,"%s", s);
    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    pthread_exit(NULL);
}

void databaseexit(char x){
	conn = PQconnectdb("dbname=internship_1903 host=bizkit.eu user=internship_1903 password=nxHeKuQzP5");
    if (PQstatus(conn) == CONNECTION_BAD)
        {
            puts("We were unable to connect to the database");
            exit(0);
        }
	sprintf(sql, "INSERT INTO public.security( timestamp, tst, alarma, tip) VALUES (localtimestamp, '%c', 'off', 'X')", x);
	res = PQexec(conn,sql);
	PQclear(res);
	PQfinish(conn);
}

void databasealarma(){
	conn = PQconnectdb("dbname=internship_1903 host=bizkit.eu user=internship_1903 password=nxHeKuQzP5");
    if (PQstatus(conn) == CONNECTION_BAD)
        {
            puts("We were unable to connect to the database");
            exit(0);
        }
	sprintf(sql, "INSERT INTO public.security( timestamp, alarma, tip) VALUES (localtimestamp, 'on', 'L')");
	res = PQexec(conn,sql);
	PQclear(res);
	PQfinish(conn);
}

void databasearmat(){
	conn = PQconnectdb("dbname=internship_1903 host=bizkit.eu user=internship_1903 password=nxHeKuQzP5");
    if (PQstatus(conn) == CONNECTION_BAD)
        {
            puts("We were unable to connect to the database");
            exit(0);
        }
	sprintf(sql, "INSERT INTO public.security( timestamp, tst, alarma, tip) VALUES (localtimestamp, '#', 'off', 'A')");
	res = PQexec(conn,sql);
	PQclear(res);
	PQfinish(conn);
}

void databasedezarmat(char passlocal1[10]){
	conn = PQconnectdb("dbname=internship_1903 host=bizkit.eu user=internship_1903 password=nxHeKuQzP5");
    if (PQstatus(conn) == CONNECTION_BAD)
        {
            puts("We were unable to connect to the database");
            exit(0);
        }
    if (state==DEZARMAT)
    {
       sprintf(sql, "INSERT INTO public.security( timestamp, tst, alarma, cod, codv, tip) VALUES (localtimestamp, 'D', 'off', '%s', 'Y', 'D')",passlocal1);
    }
    else
    {
       sprintf(sql, "INSERT INTO public.security( timestamp, tst, alarma, cod, codv, tip) VALUES (localtimestamp, 'D', 'on', '%s', 'N', 'L')",passlocal1);
    }
	res = PQexec(conn,sql);
	PQclear(res);
	PQfinish(conn);
}

void databasepinnou(char passlocal1[10]){
	conn = PQconnectdb("dbname=internship_1903 host=bizkit.eu user=internship_1903 password=nxHeKuQzP5");
    if (PQstatus(conn) == CONNECTION_BAD)
        {
            puts("We were unable to connect to the database");
            exit(0);
        }
    if (state==DEZARMAT)
    {
       sprintf(sql, "INSERT INTO public.security( timestamp, tst, alarma, cod, codv, tip) VALUES (localtimestamp, 'D', 'off', '%s', 'Y', 'C')",passlocal1);
    }
    else
    {
       sprintf(sql, "INSERT INTO public.security( timestamp, tst, alarma, cod, codv, tip) VALUES (localtimestamp, 'D', 'off', '%s', 'N', 'C')",passlocal1);
    }
	res = PQexec(conn,sql);
	PQclear(res);
	PQfinish(conn);
}


void exitvp(char x)
{
    if (x == '*')
    {
    	databaseexit(x);
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

void introducere(char x)
{
    if (x == 'A')
    {
        j++;
        if (j == 20)
        {
            state = PINV;
            j=0;
            press=1;
            printf("Introduce MasterPIN:\n");
        }
    }
    else
    {
        j=0;
    }
}

void armare(char x) // functie de armare daca tasta "#" este tinuta apasat
{
    if (x == '#')
    {
        inc++;
        if (inc == 20)
        {
            if (digitalRead(BUTTON) == HIGH)
            {
                buzzArmat();
                databasearmat();
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

void verificarebuton() // verifica daca usa/geam este deschis
{
    if (digitalRead(BUTTON) != HIGH)
    {
        button=0;
    }
    else
    {
        button=1;
    }
    if((button==0)&&(state==ARMAT))
    {
        pthread_create( &th_led, NULL, ledf, NULL );
        printf("Usa deschisa \n");
        databasealarma();
        state=ALARMA;
    }
}

void verificarePin(char x)
{
    if (press == 0)
    {
        press = 1;
        if ((x == 'D')||(i==8))
        {
            if (strcmp(admin, pass) == 0)
            {
                correct = 1;
            }
            if (correct == 1)
            {
                printf("CORRECT\n");
                state = PINI;
                pass[0] = '\0';
                i = 0;
                correct = 0;
                printf("Introduce PIN nou:\n");
            }
            else
            {
                printf("INCORRECT\n");
                //state=ALARMA;
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

void introducerePin(char x)
{
    if (press == 0) //PROBLEM!!!!!!!!!!!!!
    {
        press = 1;
        if (x == 'D')
        {
            if(strlen(passlocal)==0)
            {
                if((i>=4)&&(i<=8))
                {
                    //passlocal=pass;
                    sprintf(passlocal,"%s",pass);
                    pass[0] = '\0';
                    i = 0;
                    printf("Introduce valabilitatea PIN\nAnul:\n");
                }
                else
                {
                    printf("PIN-ul trebuie sa fie din 4-8 numere!\n");
                    pass[0] = '\0';
                    i = 0;
                }
            }
            else
            {
                if(yy==0)
                {
                    yy=atoi(pass);
                    if((yy<1900) || (yy>9999))
                    {
                        printf("Anul este incorect!\n");
                        yy=0;
                        pass[0] = '\0';
                        i = 0;
                    }else
                    {
                        pass[0] = '\0';
                        i = 0;
                        printf("Luna:\n");
                    }
                }
                else
                {
                    if(mm==0)
                    {
                        mm=atoi(pass);
                        if((mm<1) || (mm>12))
                        {
                            printf("Luna este incorecta!\n");
                            mm=0;
                            pass[0] = '\0';
                            i = 0;;
                        }else
                        {
                            pass[0] = '\0';
                            i = 0;
                            printf("Ziua:\n");
                        }
                    }
                    else
                    {
                        if(dd==0)
                        {
                            dd=atoi(pass);
                            if((dd>=1 && dd<=31) && (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12))
                            {
                                dd=atoi(pass);
                                pass[0] = '\0';
                                i = 0;
                                printf("PIN:%s DATA:%d/%02d/%02d\nApasa D pentru confirmare\n",passlocal, yy, mm, dd);
                            }
                            else if((dd>=1 && dd<=30) && (mm==4 || mm==6 || mm==9 || mm==11))
                            {
                                dd=atoi(pass);
                                pass[0] = '\0';
                                i = 0;
                                printf("PIN:%s DATA:%d/%02d/%02d\nApasa D pentru confirmare\n",passlocal, yy, mm, dd);
                            }
                            else if((dd>=1 && dd<=28) && (mm==2))
                            {
                                dd=atoi(pass);
                                pass[0] = '\0';
                                i = 0;
                                printf("PIN:%s DATA:%d/%02d/%02d\nApasa D pentru confirmare\n",passlocal, yy, mm, dd);
                            }
                            else if(dd==29 && mm==2 && (yy%400==0 ||(yy%4==0 && yy%100!=0)))
                            {
                                dd=atoi(pass);
                                pass[0] = '\0';
                                i = 0;
                                printf("PIN:%s DATA:%d/%02d/%02d\nApasa D pentru confirmare\n",passlocal, yy, mm, dd);
                            }
                            else
                            {
                                printf("Ziua este incorecta!\n");
                                dd=0;
                                pass[0] = '\0';
                                i = 0;
                            }
                        }
                        else
                        {
                            sprintf(dataintrodusa,"%d/%02d/%02d", yy, mm, dd);
                            if(strcmp(dataintrodusa, azi) >= 0)
                            {
                                sprintf(date,"%s %d/%02d/%02d\n",passlocal, yy, mm, dd); //creare string cu format data
                                fp = fopen("parole.txt", "a"); //deschidere fisier in mod append pentru a adauga la sfarsitul lui parolele

                                fputs(date, fp);
                                fclose (fp);
                                pass[0] = '\0';
                                i = 0;
                                yy=mm=dd=0;
                                state=DEZARMAT;
                                databasepinnou(passlocal);
                                printf("PIN-ul nou a fost introdus\nApasa # pentru armare\nApasa A pentru PIN nou\n");
                            }
                            else
                            {
                                printf("PIN-ul nou nu a putut fi introdus!\nIntroduce PIN nou:\n");
                                databasepinnou(passlocal);
                                passlocal[0]='\0';
                                pass[0] = '\0';
                                i = 0;
                                yy=mm=dd=0;
                            }
                        }
                    }
                }
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


void verificare(char x)
{
    if (digitalRead(BUTTON) == LOW)
    {
        verificarebuton();
    }
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
                printf("Dezarmat\nApasa # pentru armare\nApasa A pentru PIN nou\n");
                state = DEZARMAT;
                databasedezarmat(pass);
                digitalWrite(BUZZER, LOW);
                delay(100);
                digitalWrite(BUZZER, HIGH);
                passlocal[0]='\0';
                pass[0] = '\0';
                i = 0;
                correct = 0;
            }
            else
            {
                /* Create led thread */
                pthread_create( &th_led, NULL, ledf, NULL );
                printf("INCORRECT\n");
                databasealarma();
                state=ALARMA;
                databasedezarmat(pass);
                digitalWrite(BUZZER, LOW);
                delay(300);
                digitalWrite(BUZZER, HIGH);
                passlocal[0]='\0';
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
    pinMode(BUTTON, INPUT);
    pinMode(BUZZER, OUTPUT);

    digitalWrite(BUZZER, HIGH);
    digitalWrite(LEDV, LOW);

    struct sockaddr_in servaddr;


    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.108.63");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    //func(sockfd);

    init_keypad();
    char x;

    while (exitv!=1)
    {
        x = get_key();
        switch(state)
        {
        case START:
            printf("System ON!\nApasa # pentru armare\nApasa A pentru PIN nou\n");
            start();
            pthread_create( &th_clientsend, NULL, clientsend, NULL );
            break;

        case DEZARMAT:
            correct=1;
            sprintf(s,"Dezarmat");
            if(x)
            {
                exitvp(x);
                introducere(x);
                armare(x);
            }
            delay(50);
            break;

        case PINV:
            correct=0;
            if(x)
            {
                exitvp(x);
                verificarePin(x);
            }
            else
            {
                press = 0;
                increment = 0;
            }
            delay(50);
            break;

        case PINI:
            if(x)
            {
                exitvp(x);
                introducerePin(x);
            }
            else
            {
                press = 0;
                increment = 0;
            }
            delay(50);
            break;

        case ARMAT:
            correct=0;
            sprintf(s,"Armat");
            pthread_create( &th_senzormiscare, NULL, senzormiscare, NULL );
            verificarebuton();
            if(x)
            {
                exitvp(x);
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
        	sprintf(s,"ALARMA");
            verificarebuton();
            if(x)
            {
                exitvp(x);
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
        	//sprintf(s,"Oprire");
            exitv=1;
            delay(1000);
            close(sockfd);

            break;
        }
    }
    digitalWrite(LEDV, HIGH);
    digitalWrite(LEDR, HIGH);
    printf("System OFF\n");
    return 0;
}
