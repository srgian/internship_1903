#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdint.h>
#include <wiringPi.h>




#define MAX_MSG 25
#define SERVER_ADDR "192.168.108.63"
#define SERVER_PORT 8080

int num1,n, num2, sum, newSd;
char line[MAX_MSG];
int sd, newSd, c, con_no;
struct sockaddr_in server, client;


//the thread function
void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}
void inclinarex(int x)
{
    int grade = x/180;
    gotoxy(174,13);
    printf("%d grade",grade);
    switch(grade)
    {
    case -15 ... 15: // 0 grade
        gotoxy(130,13);
        printf(" _______________________________________");
        break;
    case 16 ... 37: // 30 grade
        for (int i=1; i<40; i++)
        {
            if(i%5==0)
            {
                gotoxy(130+i,18-(i/5));
                printf("/");
            }
            else
            {
                gotoxy(130+i,17-(i/5));
                printf("_");
            }
        }
        break;
    case 38 ... 53: // 45 grade
        for (int i=1; i<30; i++)
        {
            if(i%2==0)
            {
                gotoxy(134+i,21-(i/2));
                printf("/");
            }
            else
            {
                gotoxy(134+i,20-(i/2));
                printf("_");
            }

        }

        break;

    case 54 ...75: // 60 grade
        for(int i=1; i<18; i++)
        {
            gotoxy(140+i,22-i);
            printf("/");
        }

        break;

    case 76 ... 100: // 90 grade
        for(int i=1; i<21; i++)
        {
            gotoxy(150,24-i);
            printf("|");
        }
        break;
    case -37 ... -16: // -30 grade
        for (int i=1; i<40; i++)
        {
            if(i%5==0)
            {
                gotoxy(130+i,9+(i/5));
                printf("\\");
            }
            else
            {
                gotoxy(130+i,9+(i/5));
                printf("_");
            }
        }
        break;
    case  -53 ... -38: // -45 grade
        for (int i=1; i<30; i++)
        {
            if(i%2==0)
            {
                gotoxy(135+i,6+(i/2));
                printf("\\");
            }
            else
            {
                gotoxy(135+i,6+(i/2));
                printf("_");
            }
        }
        break;
    case -75 ... -54: // -60 grade
        for(int i=1; i<20; i++)
        {
            gotoxy(140+i,3+i);
            printf("\\");
        }
        break;
    case -100 ... -76: // -90 grade
        for(int i=1; i<21; i++)
        {
            gotoxy(150,3+i);
            printf("|");
        }
        break;



    default:
        gotoxy(150,12);
        printf("erroare");
    }
}

void accx(int x)
{
    if (x<0) //verificam daca x este pozitiv sau negativ sa miscam la stanga sau la dreapta nivelul
    {
        int xx= abs(x);

        int xstep=xx/1300; //un character apare la fiecare 1300 de unitati
        gotoxy(30,20); // pozitia
        printf("|");
        for(int i=1; i<=xstep; ++i) //desenam o linie pentru fiecare unitate
        {
            gotoxy(30-i,20);
            printf("-");

        }
    }
    else
    {
        int xstep= x/1300;
        gotoxy(30,20);
        printf("|");
        for(int i=1; i<=xstep; ++i)
        {
            gotoxy(30+i,20);
            printf("-");
        }
    }
}


void accz(int x)
{
    if (x<0) //verificam daca x este pozitiv sau negativ sa miscam la stanga sau la dreapta nivelul
    {
        int xx= abs(x);

        int xstep=xx/1300; //un character apare la fiecare 1300 de unitati
        gotoxy(30,24); // pozitia
        printf("|");
        for(int i=1; i<=xstep; ++i) //desenam o linie pentru fiecare unitate
        {
            gotoxy(30-i,24);
            printf("-");

        }
    }
    else
    {
        int xstep= x/1300;
        gotoxy(30,24);
        printf("|");
        for(int i=1; i<=xstep; ++i)
        {
            gotoxy(30+i,24);
            printf("-");
        }
    }
}


void accy(int x)
{
    if (x<0) //verificam daca x este pozitiv sau negativ sa miscam la stanga sau la dreapta nivelul
    {
        int xx= abs(x);

        int xstep=xx/1300; //un character apare la fiecare 1300 de unitati
        gotoxy(300,22); // pozitia
        printf("|");
        for(int i=1; i<=xstep; ++i) //desenam o linie pentru fiecare unitate
        {
            gotoxy(30-i,22);
            printf("-");

        }
    }
    else
    {
        int xstep= x/1300;
        gotoxy(30,22);
        printf("|");
        for(int i=1; i<=xstep; ++i)
        {
            gotoxy(30+i,22);
            printf("-");
        }
    }
}


void *connection_handler(void *);

int main(int argc, char *argv[])
{
    int cliLen;
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );


    //Create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");


    //Bind
    if( bind(sd,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(sd, 3);


    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;
    con_no=0;
    //printf("A \n");
    while(1)
    {
        cliLen = sizeof(client);
        newSd=accept(sd, (struct sockaddr *)&client, &cliLen) ;
        if (con_no<3)
        {
            puts("Connection accepted");
            con_no++;
            if( pthread_create( &thread_id, NULL,  connection_handler, (void*) &newSd) < 0)
            {
                perror("could not create thread");
                con_no--;
                return 1;
            }

            //Now join the thread , so that we dont terminate before the thread
            //pthread_join( thread_id , NULL);
            puts("Handler assigned");
        }
        else
            perror("Too many connections!\n");
    }
    if (newSd < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}

/*
 * This will handle connection for each client
 * */

void *connection_handler(void *sd)
{
    //Get the socket descriptor
    int sock = *(int*)sd;
    int read_size;
    char *message, client_message[25];

    //Send some messages to the client
    //  message = "Greetings! I am your connection handler\n";
    //  write(sock, message, strlen(message));


    //Receive a message from client
    do
    {
        memset(line,0x0,MAX_MSG);

        n=recv(newSd, line, MAX_MSG, 0);
        float gx,gy,gz,ax,ay,az=5000;
        float temp;
        memcpy(&gx, &line[1], sizeof(gx));
        memcpy(&gy, &line[5], sizeof(gy));
        memcpy(&gz, &line[9], sizeof(gz));
        memcpy(&ax, &line[13], sizeof(ax));
        memcpy(&ay, &line[17], sizeof(ay));
        memcpy(&az, &line[21], sizeof(az));
        // memcpy(&temp, &line[26], sizeof(temp));
//1
        system("clear");

        // display accel/gyro x/y/z values
        gotoxy(5,12);
        printf("a/g: %f %f %f   %f %f %f\n",gx,gy,gz,az,ay,az);
        gotoxy(94,24);
        printf("| Axa X - Inclinatia si acceleratia");
        gotoxy(93,24);
        printf("Valori");
//inet_addr


        accx(ax);
        accy(ay);
        accz(az);
        inclinarex(gx); //1
//      printf("received from host [IP %s ,TCP port %d] : \n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        //send(newSd, line1, strlen(line1) + 1, 0);
        //   printf("header= %0x%02X gx=%f gy=%f gy=%f ax=%f ay=%f az%f",line[0],gx,gy,gz,ax,ay,az);
        //    printf("received from host 2 [IP %s ,TCP port %d] (%d) : header: 0x%02X; GX=0x%02X%02X%02X%02X\n",
        //           inet_ntoa(client.sin_addr), ntohs(client.sin_port), n, line[0], line[1], line[2], line[3], line[4]);

    }
    while(abs(strcmp(line, "quit")));

    //Send the message back to client
    //   write(sock , client_message , strlen(client_message));

    //clear the message buffer
//	memset(client_message, 0, 25);


    if (read_size==0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size==-1)
    {
        perror("recv failed");

    }
    con_no--;
    return 0;
}
