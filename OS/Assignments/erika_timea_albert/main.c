#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#define MAGIC 4
#define HEADER 6
struct Section{
    char name[7];
    int size;
    int offset;
    short type;
};
int input_parse(int list,int parse,int extract,int find_all,int recursive,char* path,char*filter_perm,char *filter_size,int sec,int lin){
    if((list+parse+extract+find_all)==0 || (list+parse+extract+find_all)>1){
        return -2;
    }
    if(strlen(path)==0) {
        return -1;
    }
    else if(list==1){
        if(lin!=-1 || sec!=-1){
            return -2;
        }

        if(recursive==0){
            return 0;
        }
        else if(recursive==1){
            return 1;
        }
    }
    else if(parse==1){
        if(recursive==1 || strlen(filter_perm)!=0 || strlen(filter_size)!=0 || sec!=-1 || lin!=-1){
            return -2;
        }
        return 2;
    }
    else if(extract==1){
        if(recursive==1 || strlen(filter_perm)!=0 || strlen(filter_size)!=0 || sec==-1 || lin==-1){
            return -2;
        }
        return 3;
    }
    return -5;


}

int test_filter(char* path,int filt_type, char*filter, struct stat inode){
    char *permissions = (char*)malloc(10*sizeof(char));
    strcpy(permissions,"");
    if(filt_type==1){
        if(!S_ISDIR(inode.st_mode)){
            if(inode.st_size<atoi(filter)){
                return 1;
            }
        }
        else{
            return 0;
        }
    }
    else if(filt_type==0) {
        (inode.st_mode & S_IRUSR) ? strcat(permissions, "r") : strcat(permissions, "-");
        (inode.st_mode & S_IWUSR) ? strcat(permissions, "w") : strcat(permissions, "-");
        (inode.st_mode & S_IXUSR) ? strcat(permissions, "x") : strcat(permissions, "-");
        (inode.st_mode & S_IRGRP) ? strcat(permissions, "r") : strcat(permissions, "-");
        (inode.st_mode & S_IWGRP) ? strcat(permissions, "w") : strcat(permissions, "-");
        (inode.st_mode & S_IXGRP) ? strcat(permissions, "x") : strcat(permissions, "-");
        (inode.st_mode & S_IROTH) ? strcat(permissions, "r") : strcat(permissions, "-");
        (inode.st_mode & S_IWOTH) ? strcat(permissions, "w") : strcat(permissions, "-");
        (inode.st_mode & S_IXOTH) ? strcat(permissions, "x") : strcat(permissions, "-");
        if (strcmp(permissions, filter) == 0) {
            return 1;
        }
    }
    return 0;

}

void print_content(char * filt_size,char*filt_perm, char *path){
    DIR* directory;
    char title[1000];
    struct dirent *entry;
    struct stat inode;

    directory = opendir(path);
    if(directory == 0){
        printf("ERROR \ninvalid directory path");
        exit(1);
    }
    printf("SUCCESS\n");
    while((entry = readdir(directory))!=0){
        sprintf(title,"%s/%s",path,entry->d_name);
        lstat(title,&inode);
        if((S_ISDIR(inode.st_mode) || S_ISREG(inode.st_mode) || S_ISLNK(inode.st_mode) )&& strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0) {
            if ((strlen(filt_size) != 0 && test_filter(title, 1, filt_size, inode)) ||
                (strlen(filt_perm) != 0 && test_filter(title, 0, filt_perm, inode))) {
                printf("%s\n", title);
            }
            else if (strlen(filt_size) == 0 && strlen(filt_perm) == 0) {
                printf("%s\n", title);
            }
        }
    }
}

void print_recursive(char*path,char *origPath, char*filter_size,char*filter_perm){
    DIR *directory;
    char * title = (char*) malloc(1000*sizeof(char));
    struct dirent * entry;
    struct stat inode;

    lstat(path,&inode);
    if(!S_ISDIR(inode.st_mode)){
        if ((strlen(filter_size)!=0 && test_filter(title,1, filter_size, inode)) ||(strlen(filter_perm)!=0 && test_filter(title,0, filter_perm, inode))) {
            printf("%s\n", path);
        }
        else if(strlen(filter_size)==0 && strlen(filter_perm)==0){
            printf("%s\n", path);
        }
        return;
    }
    else{

        directory = opendir(path);
        if (strcmp(path, origPath) != 0 && ((strlen(filter_size)!=0 && test_filter(title,1, filter_size, inode)) ||(strlen(filter_perm)!=0 && test_filter(title,0, filter_perm, inode)))) {
            printf("%s\n", path);
        }

        else if(strcmp(path, origPath) != 0 && strlen(filter_size)==0 && strlen(filter_perm)==0){
            printf("%s\n", path);
        }
        while((entry = readdir(directory))!=0){
            sprintf(title,"%s/%s",path,entry->d_name);
            if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0  ) {
                print_recursive(title, origPath,filter_size,filter_perm);
            }
        }
        free(title);

    }


}

int test_SF(char * path, struct Section sections[],char *section_nr,short *version){
    struct stat inode;
    int fd,i=0;
    char magic[5];
    magic[4]='\0';
    short header;
    char name[7];
    int size;
    int offset;
    short type;
    lstat(path,&inode);
    if(S_ISDIR(inode.st_mode)){
        printf("ERROR\ninvalid SF path");
        exit(4);
    }
    if((fd = open(path,O_RDONLY))<0){
        printf("\nCannot open file");
        exit(5);
    }
    lseek(fd,-1*MAGIC,SEEK_END);
    if(read(fd,magic,4)<0){
        printf("Couldn't read magic");
        exit(6);
    }
    if(strcmp(magic,"Z78m")!=0){

        return 1 ;
    }
    lseek(fd,-1*HEADER,SEEK_END);
    if(read(fd,&header,2)<0){
        printf("Couldn't read header");
        exit(7);
    }
    lseek(fd, -1*(header),SEEK_END);
    if(read(fd,version,2)<0){
        printf("Couldn't read version");
        exit(8);
    }
    if(*version<75 || *version >161){
        return 2;
    }
    if(read(fd,section_nr,1)<0){
        printf("Couldn't read section number");
        exit(9);
    }
    if(*section_nr<3 || *section_nr>14){
        return 3;
    }
    while(i<(*section_nr)){
        read(fd,name,7);
        read(fd,&type,2);
        read(fd,&offset,4);
        read(fd,&size,4);
        strcpy(sections[i].name,name);
        sections[i].type=type;
        sections[i].offset=offset;
        sections[i].size=size;
        if(sections[i].type!=42 && sections[i].type!=44){
            return 4;
        }
        i++;

    }
    close(fd);
    return 0;

}
void print_parse_result(int j,short version,char nr_section,struct Section sections[] ){
    switch (j) {
        case 0:
            printf("SUCCESS\nversion=%d\nnr_sections=%d\n",version,nr_section);
            for(int i=0;i<nr_section; i++){
                printf("section%d: %s %d %d \n",i+1,sections[i].name,sections[i].type,sections[i].size);
            }
            break;
        case 1:
            printf("ERROR\nwrong magic");
            break;
        case 2:
            printf("ERROR\nwrong version");
            break;
        case 3:
            printf("ERROR\nwrong sect_nr");
            break;
        case 4:
            printf("ERROR\nwrong sect_types");
            break;
        default:
            printf("Something went wrong");
    }

}

int get_line(int fd,int offset, char * buf, int line_no){
    int i=0;
    int current_line=0;
    char line[1000];
    lseek(fd,offset,SEEK_SET);
    while(read(fd,&line[i],1)!=0 && current_line<line_no ){
        if(line[i]=='\n'){
            current_line++;
            if(current_line<line_no){
                i=0;
            }
        }
        else{
            i++;
        }
    }
    if(current_line < line_no) return -1;
    line[i]='\0';
    strcpy(buf,line+i);
    return i;
}
void extract_line(char *path,struct Section sections[],char nr_section,int sec,int lin){
    int fd,offset,len;
    char *line=(char*)malloc(1000*sizeof(char));
    if(sec>nr_section){
        printf("ERROR\ninvalid section");
        return;
    }
    if((fd=open(path,O_RDONLY))<0){
        printf("Cannot open file");
        exit(5);
    }
    offset=sections[sec-1].offset;
    len=get_line(fd,offset,line,lin);
    if(len == -1){
        printf("ERROR\ninvalid line");
        return;
    }
    // printf("%d %d",sec,lin);
    printf("SUCCESS\n");
    for(int i=strlen(line)-1;i>=0;i--){
        printf("%c",line[i]);
    }
    free(line);
    close(fd);

}


int main(int argc, char **argv){
    struct stat inode;
    struct Section sections[15];
    char * path,* filter_size,*filter_perm,nr_section;
    int list, parse,extract,recursive,find_all,operation,j;
    short version,sec,lin;

    if(argc >= 2) {
        if (argc == 2) {
            if (strstr(argv[1], "variant")) {
                printf("59153\n");
            } else {
                printf("Not a valid command!\n");
                exit(0);
            }
        } else {
            list = 0;
            parse = 0;
            extract = 0;
            recursive = 0;
            find_all = 0;
            lin = -1;
            sec = -1;
            path = (char *) malloc(1000 * sizeof(char));
            filter_size = (char *) malloc(1000 * sizeof(char));
            filter_perm = (char *) malloc(1000 * sizeof(char));
            for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], "list") == 0) {
                    list = 1;
                } else if (strcmp(argv[i], "parse") == 0) {
                    parse = 1;
                } else if (strcmp(argv[i], "extract") == 0) {
                    extract = 1;
                } else if (strcmp(argv[i], "findall") == 0) {
                    find_all = 1;
                } else if (strcmp(argv[i], "recursive") == 0) {
                    recursive = 1;
                } else if (strstr(argv[i], "path=")) {
                    strcpy(path, argv[i] + 5);
                } else if (strstr(argv[i], "size_smaller=")) {
                    strcpy(filter_size, argv[i] + 13);
                } else if (strstr(argv[i], "permissions=")) {
                    strcpy(filter_perm, argv[i] + 12);
                } else if (strstr(argv[i], "section=")) {
                    sec = atoi(argv[i] + 8);
                } else if (strstr(argv[i], "line=")) {
                    lin = atoi(argv[i] + 5);
                } else {
                    printf("Not a valid command!\n");
                    exit(1);
                }
            }
            operation = input_parse(list, parse, extract, find_all, recursive, path, filter_perm, filter_size, sec, lin);
            switch (operation) {
                case -5:
                    printf("Something terribly went wrong!\n");
                    break;
                case -2:
                    printf("Not a valid command!\n");
                    break;
                case -1:
                    printf("Not a valid path!");
                    break;
                case 0:
                    print_content(filter_size, filter_perm, path);
                    free(path);
                    break;
                case 1:
                    if (lstat(path, &inode) < 0) {
                        printf("ERROR\ninvalid directory path");
                        exit(2);
                    }
                    printf("SUCCESS\n");
                    print_recursive(path, path, filter_size, filter_perm);
                    break;
                case 2:
                    j = test_SF(path, sections, &nr_section, &version);
                    print_parse_result(j,version,nr_section,sections);
                    free(path);
                    break;
                case 3:
                    j = test_SF(path, sections, &nr_section, &version);
                    if (j != 0) {
                        printf("ERROR\ninvalid file");
                        printf("%d ", j);
                        exit(9);
                    }
                    extract_line(path, sections, nr_section, sec, lin);
                    free(path);
                    break;
                default:;

            }
            free(filter_size);
            free(filter_perm);

        }
    }

    return 0;
}