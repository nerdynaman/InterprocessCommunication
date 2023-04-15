#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include<semaphore.h>
#include<fcntl.h>

sem_t *mutex1,*mutex2;

int main(){
    mutex1 = sem_open("m1", O_CREAT, 0644, 1);
    mutex2 = sem_open("m2", O_CREAT, 0644, 0);
    if (mutex1 == SEM_FAILED || mutex2 == SEM_FAILED){
        perror("error in sem_open");
        exit(1);
    }
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
    
    char* messageArray = malloc(sizeof(4));
    int lCount = 0;
    int wCount = sizeof(int);
    int id;
    if (sem_wait(mutex1) < 0){
        perror("error in sem_wait");
    }
    for (int i = 0 ; i < 50 ; i++){
        strncpy(&id, data+wCount, 4);
        strncpy(messageArray, data+wCount+4, 4);
        wCount+=8;
        lCount++;
        printf("%d ",id);
        printf("%s \n",messageArray);
        if (lCount%5==0){
            strncpy(data,&lCount, sizeof(lCount));
            if (sem_post(mutex1) < 0){
                perror("error in sem_post");
            }
            if(sem_post(mutex2)<0){
                perror("error in sem_post");
            }
            if (sem_wait(mutex2) < 0){
                perror("error in sem_wait");
            }
            if (sem_wait(mutex1) < 0){
                perror("error in sem_wait");
            }
        }
    }
    return 0;
}