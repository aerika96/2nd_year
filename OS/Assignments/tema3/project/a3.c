#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    int fd_req, fd_resp;
    char buf[100];

    if((fd_req = open("REQ_PIPE_59153", O_RDONLY))< 0) {
        printf("ERROR\ncannot open request pipe");
        exit(2);
    }

    if (mkfifo("RESP_PIPE_59153", 0640) < 0) {
        printf("ERROR\ncannot create the response pipe");
        exit(1);
    }

    if ((fd_resp = open("RESP_PIPE_59153", O_WRONLY)) < 0) {
        printf("ERROR\ncannot open response pipe");
        exit(2);
    }
    strcpy(buf, "07434f4e4e454354");
    if (write(fd_resp, buf, strlen("07434f4e4e454354")) < 0) {
        printf("ERROR\ncannot write in response pipe");
        exit(2);
    }
    printf("SUCCESS\n");
    return 0;
}

