#include <stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>

int search_dir(char * path, char*file){
    DIR *directory;
    struct dirent * entry;
    struct stat inode;
    char* abs_path=(char*)malloc(1000*sizeof(char));

    directory= opendir(path);
    if(directory ==0 ){
        perror("Cannot open directory!");
        exit(3);
    }
    while(entry=readdir(directory)){
        sprintf(abs_path,"%s/%s ",path,entry->d_name);
        if(strcmp(entry->d_name,file)==0){
            printf("File found on the path : %s\n", abs_path);
            lstat(abs_path,&inode);
            if(S_ISDIR(inode.st_mode)){
                printf("This entry is a directory");
            }
            else if(S_ISREG(inode.st_mode)){
                printf("This entry is a regular file");
            }
            else if(S_ISLNK(inode.st_mode)) {
                printf("This entry is a link");
            }
            else if(S_ISFIFO(inode.st_mode)){
                printf("This entry is a pipe file");
            }
            else if(S_ISBLK(inode.st_mode)){
                printf("This entry is a special device of type block");
            }
            closedir(directory);
            free(abs_path);
            return 1;
        }
    }
    closedir(directory);
    free(abs_path);
    return 0;

}

int search_tree(char * path, char *file){
    struct stat inode;
    DIR *directory;
    struct dirent * entry;
    char* abs_path=(char*)malloc(1000*sizeof(char));
    lstat(path,&inode);

    directory=opendir(path);
    if(directory==0){
        printf("Cannot open directory");
        exit(4);
    }

    if(search_dir(path,file)==1){
        return 1;
    }
    while(entry=readdir(directory)){
        sprintf(abs_path, "%s/%s", path, entry->d_name);
        lstat(abs_path,&inode);
        if(S_ISDIR(inode.st_mode) && strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0 ){
            return search_tree(abs_path,file);
        }
    return 0;
    }

}

int main(int argc, char **argv) {
    struct stat inode;

    if(argc<3)
    {
        perror("INCORRECT INPUT DATA! CORRECT FORM: <file_path> <file_to_search>");
        exit(1);
    }
    else{
        printf("path= %s , name= %s", argv[1], argv[2]);
        lstat(argv[1],&inode);
        if(!S_ISDIR(inode.st_mode)&& !S_ISREG(inode.st_mode)){
            perror("Incorrect directory path");
            exit(2);
        }
        if(search_dir(argv[1],argv[2])==0){
            printf("File not found in the given directory");
        }
        if(search_tree(argv[1],argv[2])==0){
            printf("File not found in the entire tree");
        }

    }
    return 0;
}