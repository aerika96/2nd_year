#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<wait.h>
int main() {

    int childPid,state;
    printf("I started\n");
    printf("I am process P1 %d\n",getpid());
    childPid=fork(); // P2
    switch(childPid){
        case -1: exit(1);
        case 0:
            // P2
            printf("I am at process P2 %d, with parent %d\n",getpid(),getppid());
            exit(0);
        default: // P1
                childPid=fork() ; // P3
                // P1 + P3
                switch(childPid){
                    case -1: exit(1);
                    case 0: // P3
                       printf("I am at process P3 %d with parent %d\n",getpid(),getppid());
                        childPid= fork(); //P5 created
                        switch(childPid){
                            case -1:
                                exit(1);
                            case 0: //P5
                                printf("I am the process P5 %d with parent %d\n",getpid(),getppid());
                                exit(0);
                            default:
                                wait(&state);

                        }
                        break;
                    default:
                    // P1
                        childPid=fork(); // P4 created
                        //P1+P3+P4
                    switch (childPid){
                        case -1:
                            exit(1);
                        case 0:
                            printf("I am at process P4 %d with parent %d\n",getpid(),getppid());
                        default:;
                            wait(&state);
                            wait(&state);
                            wait(&state);
                    }


                }
    }


    return 0;
}