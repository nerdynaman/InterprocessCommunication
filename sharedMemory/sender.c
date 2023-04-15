#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<semaphore.h>
#include<fcntl.h>
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
        internalMessageStruct->mesg[sizeOfEachMessage] = '\0';
        internalMessageStruct->id = i;
        messageArray[i] = internalMessageStruct;
    }
    return messageArray;
}

sem_t *mutex1, *mutex2;
int main(){
    struct timespec ts1, ts2;
    clock_gettime(CLOCK_REALTIME, &ts1);
    int a = sem_unlink("m1");
    if (a<0){
        perror("error in sem_unlink");
    }
    a = sem_unlink("m2");
    if (a<0){
        perror("error in sem_unlink");
    }
    mutex1 = sem_open("m1", O_CREAT, 0644, 1);
    mutex2 = sem_open("m2", O_CREAT, 0644, 0);
    if (mutex1 == SEM_FAILED || mutex2 == SEM_FAILED){
        perror("error in sem_open");
        exit(1);
    }
    // int res = 0;
    // int nko = sem_getvalue(&mutex1,&res);
    // if (nko<0){
    //     perror("error in get value");
    // }
    // printf("%d",nko);
    // printf("%d",res);
    key_t key = ftok("io", 'r');
    if (key == -1){
        perror("error in ftok");
    }
    int shmid = shmget(key, 4096, 0666|IPC_CREAT);
    if (shmid == -1){
        perror("error in shmget");
    }
    char *data = shmat(shmid, (void *)0, 0);
    if (data == (char *)-1){
        perror("error in shmat");
    }
    // int data = shm_open("new",O_CREAT,0666);
    msg** messageArray = generateMessageArray(3);
    int lCount = 0;
    int wCount = sizeof(int);
    if (sem_wait(mutex1) < 0){
        perror("error in sem_wait");
    }
    for (int i = 0; i < 50; i++){ 
        printf("%d start ",lCount);
        strncpy(data+wCount, &messageArray[i]->id, 4);
        strncpy(data+wCount+4, messageArray[i]->mesg, 4);
        wCount+=8;
        lCount++;
        printf("%d done\n",lCount);
        if(lCount%5==0){
            if (sem_post(mutex1) < 0){
                perror("error in sem_post");
            }
            if (lCount==50){
                break;
            }
            if(sem_wait(mutex2) < 0){
                perror("error in sem_wait");
            }
            char *d = (char*)malloc(sizeof(int));
            strncpy(d, data, sizeof(int));
            lCount = *(int*)d;
            printf("Received %d\n", lCount);
            if (sem_wait(mutex1) < 0){
                perror("error in sem_wait");
            }
            if (sem_post(mutex2) < 0){
                perror("error in sem_wait");
            }
        }
    }
    if (sem_post(mutex2) < 0){
        perror("error in sem_post");
    }
    clock_gettime(CLOCK_REALTIME, &ts2);
    float time = (ts2.tv_sec - ts1.tv_sec) + (ts2.tv_nsec - ts1.tv_nsec)/1000000000.0;
    printf("Time taken: %f", time);
    return 0;
}