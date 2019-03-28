//execute with: sudo g++ -I . I2Cdev.cpp MPU6050.cpp demo_raw.cpp -o demo_raw2 -lwiringPi
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <arpa/inet.h>
#include <wiringPi.h>

#define MAX 25
#define PORT 8080
#define SA struct sockaddr

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup()
{
    // initialize device
    printf("Initializing I2C devices...\n");
    accelgyro.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(accelgyro.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");
}

void func(int sockfd, float ax, float ay, float az, float gx, float gy, float gz)
{
    char buff[MAX];
    int n;
    char header=0x01;

        bzero(buff, sizeof(buff));
        memcpy(&buff[0], &header, sizeof(header));
        memcpy(&buff[1], &ax, sizeof(ax));
       // printf("GX=0x%02X%02X%02X%02X\n", buff[1], buff[2], buff[3], buff[4]);
        memcpy(&buff[5], &ay, sizeof(ay));
        memcpy(&buff[9], &az, sizeof(az));
        memcpy(&buff[13], &gx, sizeof(gx));
        memcpy(&buff[17], &gy, sizeof(gy));
        memcpy(&buff[21], &gz, sizeof(gz));
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        //read(sockfd, buff, sizeof(buff));
       // printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0)
        {
            printf("Client Exit...\ninet_addr");

        }
        delayMicroseconds(100000);
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
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

    // connect the client socket to server socket.
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    setup();
    for (;;)
    {
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        func(sockfd, ax, ay, az, gx, gy, gz);
    }
    // function for chat
    //func(sockfd, 1.1f, 1.2f, 1.3f, 2.1f, 2.2f, 2.3f);

    // close the socket
    close(sockfd);
}
