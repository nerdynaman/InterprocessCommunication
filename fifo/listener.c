#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

typedef struct msg{
    char mesg[11];
    int id;
} msg;

int main(){
    mknod("data", S_IFIFO | 0666, 0);
    mknod("feedback", S_IFIFO | 0666, 0);
    int fd = open("data", O_RDONLY);
    int fd2 = open("feedback", O_WRONLY);
    msg *messageArray = malloc(sizeof(msg));
    int lCount = 0;
    for (int i = 0 ; i < 50 ; i++){
        int a =read(fd,messageArray, sizeof(msg));
        if (a < 0){
            // perror("error in recv");
            printf("Error in recv%d",a);
        }
        else{
            lCount++;
            printf("%d ",messageArray->id);
            printf("%s \n",messageArray->mesg);
        }
        if (lCount%5==0){
            if (write(fd2,&lCount, sizeof(lCount))<0){
                perror("error in send");
            }
        }
    }
    close(fd);
    close(fd2);
    return 0;
}