#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

typedef  struct _section{
    char name[7];
    short type;
    int size;
    int offset;
}SectionT;


int fd_req, fd_resp,shm_id,dimension,file_size;
char *addr;
void *file_start;
//section *sections;


char* read_request(){
    unsigned char req_length;
    char *request;

    read(fd_req, &req_length, sizeof(unsigned char));
    request = (char *) malloc((req_length + 1) * sizeof(char));
    read(fd_req, request, req_length);
    request[req_length] = '\0';
    return request;

}
void write_response( char* message){
    unsigned char size=strlen(message);
    if(write(fd_resp,&size,sizeof(unsigned char))<0){
        printf("ERROR\ncannot write size");
        return;
    }
    if(write(fd_resp,message,size)<0){
        printf("ERROR\ncannot write message");
        return;
    }
}

void handle_ping(){
    int var = 59153;;

    write_response("PING");
    write_response("PONG");
    write(fd_resp, &var, sizeof(int));
}

void handle_create_shm(){


    if(read(fd_req,&dimension,sizeof(int))<0){
        printf("ERROR\ncannot read shared memory dimension");
        return;
    }
    write_response("CREATE_SHM");
    shm_id=shmget(11540,dimension,IPC_CREAT|0664);
    if(shm_id<0){
        write_response("ERROR");
        return;
    }
    addr = (char*)shmat(shm_id,0,0);
    if(addr<(char *)0){
        printf("ERROR\ncannot attach shm area");
        write_response("ERROR");
        return;
    }

    write_response("SUCCESS");
}

void handle_write_shm(){
    unsigned int offset,value;
    int *pointer_to_int;

    if(read(fd_req,&offset,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read offset");
        return;
    }
    if(read(fd_req,&value,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read value");
        return;
    }

    write_response("WRITE_TO_SHM");


    if(offset>=dimension || (offset+sizeof(unsigned int))>=dimension){
        write_response("ERROR");
        return;
    }
    pointer_to_int=(int*)(addr+offset);
    *pointer_to_int=value;
    write_response("SUCCESS");


}

void handle_map_file(){

    int fd_map;
    unsigned char path_length;
    char path[1000];
    struct stat st;

    write_response("MAP_FILE");

    if(read(fd_req,&path_length,sizeof(unsigned char))<0){
        printf("ERROR\ncannot read path length");
        return;
    }

    if(read(fd_req,path,path_length)<0){
        printf("ERROR\ncannot read path\n");

        return;
    }

    path[path_length]='\0';
    if((fd_map=open(path,O_RDONLY))<0){
        printf("Cannot open file");
        write_response("ERROR");
        return;
    }

    if(stat(path,&st)<0){
        printf("ERROR\ncannot reach stat\n");
        write_response("ERROR");
        return;
    }
    file_size=(int) st.st_size;

    file_start=mmap(NULL,(size_t)file_size,PROT_READ,MAP_SHARED,fd_map,0);
    if(file_start<(void*)0){
        write_response("ERROR");
    }
    else {
        write_response("SUCCESS");
    }

}

void handle_read_offset(){
    unsigned int offset, no_of_bytes;
    int i;
    char *content;

    if(read(fd_req,&offset,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read offset\n");
        return;
    }

    if(read(fd_req,&no_of_bytes,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read number of bytes\n");
        return;
    }

    write_response("READ_FROM_FILE_OFFSET");

    if(addr<(char*)0 || file_start<(void*)0 ||  offset+no_of_bytes>=file_size || dimension<no_of_bytes){
        printf("ERROR\nconditions not satisfied\n");
        write_response("ERROR");
        return;
    }

        content=(char*)malloc(no_of_bytes*sizeof(char));
        for(i=0;i<no_of_bytes; i++){
            content[i]=*(char*)(file_start+offset+i);
            addr[i]=content[i];
        }
        write_response("SUCCESS");

}
/*
void read_sections(unsigned int section_no){

    unsigned int standard_end=*(int*)file_start+file_size-1;
    unsigned short header_size;
    header_size=*(unsigned short*)(standard_end-4);
    printf("%d",header_size);
}

void handle_read_section(){

    unsigned int section_no,offset,no_of_bytes;

    write_response("READ_FROM_FILE_SECTION");
    if(read(fd_req,&section_no,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read section number\n");
        return;
    }
    if(read(fd_req,&offset,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read section offset\n");
        return;
    }
    if(read(fd_req,&no_of_bytes,sizeof(unsigned int))<0){
        printf("ERROR\ncannot read section number of bytes\n");
        return;
    }

    if(file_start<(void*)0 || addr<(char*)0 ){
        printf("ERROR\nconditions not satisfied\n");
        write_response("ERROR");
        return;
    }
    printf("%d %d %d",section_no,offset,no_of_bytes);
    //read_sections(section_no);
    //printf("%d",section_pos);
    write_response("ERROR");
}*/
int main(int argc, char **argv) {

    char buf[100],*request;
    unsigned char length;

    if (mkfifo("RESP_PIPE_59153",0666) < 0) {
        printf("ERROR\ncannot create the response pipe");
        exit(1);
    }
    if ((fd_req = open("REQ_PIPE_59153", O_RDONLY)) < 0) {
        perror("ERROR\ncannot open request pipe");
        exit(2);
    }
    if ((fd_resp = open("RESP_PIPE_59153", O_WRONLY)) < 0) {
        printf("ERROR\ncannot open response pipe");
        exit(2);
    }
    strcpy(buf, "CONNECT");
    length=7;
    if(write(fd_resp,&length,sizeof(unsigned char))<0){
        printf("ERROR\ncannot write response");
        exit(3);
    }
   if(write(fd_resp, buf, strlen("CONNECT"))<0){
       printf("ERROR\ncannot write response");
       exit(3);
   }
    printf("SUCCESS\n");

    while(1) {
        request=read_request();
        if ((strncmp(request, "EXIT", length + 1) == 0)) {
            break;
        } else if ((strncmp(request, "PING", length + 1) == 0)) {
            handle_ping();
        }
        else if ((strncmp(request, "CREATE_SHM", length + 1) == 0)){
           handle_create_shm();
        }
        else if((strncmp(request,"WRITE_TO_SHM",length + 1))==0){
            handle_write_shm();
        }
        else if((strncmp(request,"MAP_FILE",length+1))==0){
            handle_map_file();
        }
        else if((strncmp(request,"READ_FROM_FILE_OFFSET",length+1))==0) {
            handle_read_offset();
        }
       /* else if((strncmp(request,"READ_FROM_FILE_SECTION",length+1))==0){

            handle_read_section();

        }*/
        else {
            break;
        }
    }
    close(fd_req);
    close(fd_resp);
    unlink("RESP_PIPE_59153");
    return 0;

}