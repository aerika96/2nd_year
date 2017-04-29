#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<wait.h>
int main() {
    int var=10;
    int childid;

    printf("The value of the variable var=%d\n",var);
    childid=fork();
    printf("I am here\n");
    if(childid==0){
        var=5;
    }
    else if(childid>0){
        waitpid(childid,NULL,0);
    }
    printf("The value of the variable  var=%d and pid=%d\n",var, getpid());


    return 0;
}