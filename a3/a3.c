#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define RESP_PIPE_NAME "RESP_PIPE_69227"
#define REQ_PIPE_NAME "REQ_PIPE_69227"
#define MESSAGE "BEGIN!"

int main() {
    int respPipe, reqPipe;
    if (mkfifo(RESP_PIPE_NAME, 0666) == -1) {
        perror("ERROR\ncannot create the response pipe");
        exit(1);
    }
    reqPipe = open(REQ_PIPE_NAME, O_RDONLY);
    if (reqPipe == -1) {
        perror("ERROR\ncannot open the request pipe");
        exit(1);
    }
    respPipe = open(RESP_PIPE_NAME, O_WRONLY);
    if (respPipe == -1) {
        perror("ERROR\ncannot open the response pipe");
        exit(1);
    }    
    if (write(respPipe, MESSAGE, strlen(MESSAGE) + 1) == -1) {
        perror("ERROR\ncannot write to the response pipe");
        exit(1);
    }
    printf("SUCCESS\n");
    
    
    close(reqPipe);
    close(respPipe);
    
   
    unlink(RESP_PIPE_NAME);
    
    return 0;
}
