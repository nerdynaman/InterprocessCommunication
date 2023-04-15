#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct msg{
    char mesg[3];
    int id;
} msg;

int main(){
    struct sockaddr_un listening;
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0){
        perror("socket");
        exit(1);
    }
    listening.sun_family = AF_UNIX;
    strcpy(listening.sun_path, "sockets\0");
    socklen_t len = sizeof(listening);
    if (connect(sock, (struct sockaddr*) &listening, len) < 0){
        perror("accept");
        exit(1);
    }
    else{
        printf("Connected\n");
        int lCount = 0;
        // while(1){
        msg *messageArray = malloc(8);
        for (int i = 0 ; i < 50 ; i++){
            int a =recv(sock,messageArray, 8, 0);
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
                if (send(sock,&lCount, sizeof(lCount), 0)<0){
                    perror("error in send");
                }
            }
        }
        // }
    return 0;
    }
}