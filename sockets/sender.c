#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
typedef struct msg{
    char mesg[4];
    int id;
} msg;

msg** generateMessageArray(int sizeOfEachMessage){
    msg** messageArray = (msg**)malloc(sizeof(msg)*50);
    for(int i=0;i<50;i++){
        msg* internalMessageStruct = (msg*)malloc(sizeof(msg));
        for (int i = 0 ; i < sizeOfEachMessage ; i++)
            internalMessageStruct->mesg[i] = 97 + rand()%26;
        internalMessageStruct->mesg[3] = '\0';
        internalMessageStruct->id = i;
        // internalMessageStruct->mesg = internalMessage;
        messageArray[i] = internalMessageStruct;
    }
    return messageArray;
}


int main(){
    struct timespec ts1, ts2;
    int sizeOfEachMessage = 3;
    msg** messageArray = generateMessageArray(sizeOfEachMessage);
    struct sockaddr_un listening;
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0){
        perror("socket");
        exit(1);
    }
    listening.sun_family = AF_UNIX;
    char* path = "sockets\0";
    strncpy(listening.sun_path, path, sizeof(listening.sun_path));
    int a =unlink(path);
    if (a < 0){
        printf("Error in unlink%d",a);
    }
    if (bind(sock, (struct sockaddr *)&listening,sizeof(listening)) < 0){
        perror("bind");
        exit(1);
    }
    else{
        listen(sock, 1);

        socklen_t sizeOfListening = sizeof(listening);
        while(1){
            clock_gettime(CLOCK_REALTIME, &ts1);
            int msgsock = accept(sock, (struct sockaddr*) &listening, &sizeOfListening);
            if (msgsock == -1){
                perror("error in accept");
            }
            else{
                printf("Accepted\n");
                int lCount = 0;
                for (int i = 0; i < 50; i++){                
                        send(msgsock, messageArray[lCount], 8,0);
                        lCount++;
                        if(lCount%5==0){
                            recv(msgsock, &lCount, sizeof(int),0);
                            printf("Received %d\n", lCount);
                        }
                }
            clock_gettime(CLOCK_REALTIME, &ts2);
            float time = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec)/1000000000.0;
            printf("Time taken: %f\n", time);
            close(msgsock);
            printf("Closed\n");
            }
        }
    }
    return 0;
}