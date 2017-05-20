#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
int main(int argc, char**argv) {

    char *path1 = (char*)malloc(100*sizeof(char));
    char *path2 = (char*)malloc(100*sizeof(char));
    strcpy(path1,argv[1]);
    int fd = open(path1,O_RDWR);
    char *id = (char*)mmap(0,17,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
    for(int i=0; i<17; i++){
        printf("%c",*(id+i));
    }
    for(int i=0; i<17; i++){
        if(i%2==0) {
            *(id + i) = 'z';
        }
    }
    printf("\n");
    for(int i=0; i<17; i++){
        printf("%c",*(id+i));
    }
    char* outs_page = (char*)mmap(0,);
    return 0;
}