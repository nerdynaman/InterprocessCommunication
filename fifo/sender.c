#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

typedef struct msg{
    char mesg[11];
    int id;
} msg;

msg** generateMessageArray(int sizeOfEachMessage){
    msg** messageArray = (msg**)malloc(sizeof(msg)*50);
    for(int i=0;i<50;i++){
        msg* internalMessageStruct = (msg*)malloc(sizeof(msg));
        for (int i = 0 ; i < sizeOfEachMessage ; i++)
            internalMessageStruct->mesg[i] = 97 + rand()%26;
        internalMessageStruct->mesg[sizeOfEachMessage] = '\0';
        internalMessageStruct->id = i;
        messageArray[i] = internalMessageStruct;
    }
    return messageArray;
}

int main(){
    struct timespec ts1, ts2;
    clock_gettime(CLOCK_REALTIME, &ts1);
    int dRes = mknod("data", S_IFIFO | 0666, 0);
    int fRes = mknod("feedback", S_IFIFO | 0666, 0);
    int fd = open("data", O_WRONLY);
    int fd2 = open("feedback", O_RDONLY);
    msg** messageArray = generateMessageArray(11);

    int lCount = 0;
    for (int i = 0; i < 50; i++){                
        write(fd, messageArray[lCount],sizeof(msg));
        lCount++;
        if(lCount%5==0){
            read(fd2, &lCount, sizeof(int));
            printf("Received %d\n", lCount);
        }
    }
    close(fd);
    close(fd2);
    clock_gettime(CLOCK_REALTIME, &ts2);
    float time = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec)/1000000000.0;
    printf("Time taken: %f", time);
    return 0;
}