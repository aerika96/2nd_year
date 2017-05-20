#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int fdToLeft[2], fdToRight[2];
int buf[100];
void increment(int pid, long long MAX)
{
    long long count = 0;
    if(pid==1){
        close(fdToLeft[0]);
        close(fdToRight[1]);
    }
    if(pid==2){
        close(fdToLeft[1]);
        close(fdToRight[0]);
    }
    if(pid==1) {
        for (count = 0; count <= MAX; count++){
            read(fdToRight[0],buf,3);
            printf("Process %d at step %lld\n", pid, count);
            write(fdToLeft[1], "proc1", 3);
        }
    }
    if(pid==2){


        for (count = 0; count <= MAX; count++) {
            write(fdToRight[1],"proc2",3);
            read(fdToLeft[0],buf,3);
            printf("Process %d at step %lld\n", pid, count);
        }
    }

}
int main() {
    pipe(fdToLeft);
    pipe(fdToRight);
    int state;
    int pid=fork();//P0
    switch(pid){
        case -1:
            perror("Something went wrong");
            exit(1);
        case 0 :
            //P1
            increment(1,10);
            exit(0);
        default:
            //P0
            pid= fork();
            switch(pid){
                case -1:
                    perror("Something went wrong");
                    exit(1);
                case 0:
                    increment(2,10);
                    exit(0);
                default:
                    wait(&state);
                    wait(&state);
             }
    }
    return 0;
}