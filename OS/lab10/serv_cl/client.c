//
// Created by aerika on 05.05.2017.
//

#include <stdlib.h>
#include <stdio.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
int main(){
    char buf[3];
    scanf("%s",buf);
    if(mkfifo ("FIFO",666)<0){
        perror("Error creating pipe");
        exit(1);
    }
    int fd= open("FIFO", O_WRONLY);
    write (fd,buf, strlen(buf));


}
