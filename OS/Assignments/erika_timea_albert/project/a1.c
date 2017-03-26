#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include<ctype.h>
#define MAGIC 4
#define HEADER 6
struct Section{
    char name[7];
    int size;
    int offset;
    short type;
};

//Check if a given string is fully constructed out of digits or not -- used when filtering or extracting sections
int check_number(char* string)
{
    for(int i=0;i<strlen(string); i++){
        if(!isdigit(string[i])){
            return 0;
        }
    }
    return 1;
}

//When the arguments are given, the input has to be verfied if it staisfies the conditions of a given functionality
int input_parse(int list,int parse,int extract,int find_all,int recursive, int path_nr,char* path,char*filter_perm,char *filter_size,int sec,int lin)
{

 //if more than one functionality is set or one more than once, the function returns -->error message displayed
    if((list+parse+extract+find_all)==0 || (list+parse+extract+find_all)>1){
        return -2;
    }

 //if the path is epty or more than one path was set in the argument list, the function returns  -->error message displayed
    if(strlen(path)==0 || path_nr>1) {
        return -1;
    }

 //verifying whether the set functionality is listing or not and if the other arguments contribute to its structure
    else if(list==1){
        if(lin!=-1 || sec!=-1){
            return -2;
        }

        //simple listing
        if(recursive==0){
            return 0;
        }

        //recursive listing
        else if(recursive==1){
            return 1;
        }

        // no listing because the argument "recursive" was set more than once
        else if(recursive>1){
            return -2;
        }
    }

 //verifying whether the functionality set was parse or not and if the the other arguments contribute to its structure
    else if(parse==1){
        if(recursive>=1 || strlen(filter_perm)!=0 || strlen(filter_size)!=0 || sec!=-1 || lin!=-1){
            return -2;
        }
        return 2;
    }

 //verifying whether the functionality set was extraction or not and if the other arguments contribute to its structure
    else if(extract==1){
        if(recursive>=1 || strlen(filter_perm)!=0 || strlen(filter_size)!=0 || sec==-1 || lin==-1){
            return -2;
        }
        return 3;
    }
 //verifying whether the functionality set was finding all the section files of a given cathegory and if the other arguments contribute to its structure
    else if(find_all==1){
        if(recursive >=1 || strlen(filter_perm)!=0 ||strlen(filter_size)!=0 || sec!=-1 || lin!=-1){
            return -2;
        }
        return 4;
    }
    return -5;


}

//when the list functionality is set there may be restrictions on the printed entries, this function verifies if they correspond the set filters
int test_filter(int filt_type, char*filter, struct stat inode)
{
    char permissions[10];

    strcpy(permissions,"");

    //if the filter refers to the size, the size of the entry is compared to the given filter, if the entry is not a directory
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

    //constructing a permission string based on the permissions of an entry to verify if it is the same with the filtering permission string
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

//this function serves the simple listing functionality
void print_content(char * filt_size,char*filt_perm, char *path)
{
    DIR* directory;
    char title[PATH_MAX];
    struct dirent *entry;
    struct stat inode;

    //opening directory
    directory = opendir(path);
    if(directory == 0){
        printf("ERROR \ninvalid directory path");
        exit(1);
    }
    printf("SUCCESS\n");

    //traversing the whole directory -- printing only the "valid entries" (sub-directories, regular files, links)
    while((entry = readdir(directory))!=0){
        sprintf(title,"%s/%s",path,entry->d_name);
        lstat(title,&inode);
        if((S_ISDIR(inode.st_mode) || S_ISREG(inode.st_mode) || S_ISLNK(inode.st_mode) )&& strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0) {
            //if filters are set
            if ((strlen(filt_size) != 0 && test_filter(1, filt_size, inode)) ||
                (strlen(filt_perm) != 0 && test_filter(0, filt_perm, inode))) {
                printf("%s\n", title);
            }
            //if filters are not set
            else if (strlen(filt_size) == 0 && strlen(filt_perm) == 0) {
                printf("%s\n", title);
            }
        }
    }
    closedir(directory);

}

//this function serves the recursive listing functionality
void print_recursive(char*path,char *origPath, char*filter_size,char*filter_perm)
{
    DIR *directory;
    char title[PATH_MAX];
    struct dirent * entry;
    struct stat inode;

    //if path is not a directory --> it has to be printed (if filters are set, it has to satisfy them )
    lstat(path,&inode);
    if(!S_ISDIR(inode.st_mode)) {
        if ((strlen(filter_size) != 0 && test_filter(1, filter_size, inode)) ||
            (strlen(filter_perm) != 0 && test_filter(0, filter_perm, inode))) {
            printf("%s\n", path);
        } else if (strlen(filter_size) == 0 && strlen(filter_perm) == 0) {
            printf("%s\n", path);
        }

        return;
    }
    else{

            //if the path is the absolute path of a directory, it has to be opened ; directories are only printed if they are not the same with the original path or the . and .. special directories
            directory = opendir(path);
            if (strcmp(path, origPath) != 0 && ((strlen(filter_size)!=0 && test_filter(1, filter_size, inode)) ||(strlen(filter_perm)!=0 && test_filter(0, filter_perm, inode)))) {
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
            closedir(directory);

    }
}

//this function serves the testing of a section file when parsing functionality is set with the correct structure
int test_SF(char * path, struct Section sections[],char *section_nr,short *version)
{
    struct stat inode;
    int fd,i=0;
    char magic[5];
    magic[4]='\0';
    short header;

    lstat(path,&inode);
    //if the given path is the absolute path of a directory --> it cannot be a section file
    if(S_ISDIR(inode.st_mode)){
        printf("ERROR\ninvalid SF path");
        exit(5);
    }
    if((fd = open(path,O_RDONLY))<0){
            printf("\nCannot open file");
            exit(5);
    }
    lseek(fd,-1*MAGIC,SEEK_END);
    if(read(fd,magic,4)<0){
        printf("Couldn't read magic");
        exit(5);
    }
    if(strcmp(magic,"Z78m")!=0){

        return 1 ;
    }
    lseek(fd,-1*HEADER,SEEK_END);
    if(read(fd,&header,2)<0){
        printf("Couldn't read header");
        exit(5);
    }
    lseek(fd, -1*(header),SEEK_END);
    if(read(fd,version,2)<0){
        printf("Couldn't read version");
        exit(5);
    }
    if(*version<75 || *version >161){
        return 2;
    }
    if(read(fd,section_nr,1)<0){
        printf("Couldn't read section number");
        exit(5);
    }
    if(*section_nr<3 || *section_nr>14){
        return 3;
    }
    while(i<(*section_nr)){
        sections[i].name[7]='\0';
        read(fd,&(sections[i].name),7);
        read(fd,&(sections[i].type),2);
        read(fd,&(sections[i].offset),4);
        read(fd,&(sections[i].size),4);
        if(sections[i].type!=42 && sections[i].type!=44){
            return 4;
        }
        i++;

    }
    close(fd);
    return 0;

}

//this function prints the result of the parsing corresponding to the value returned by the testing function
void print_parse_result(int j,short version,char nr_section,struct Section sections[] )
{
    // j is the result returned by the testing function - relative to its value something is printed
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


//this function "breaks down" each section into lines based on the special new line character or the end of the section
int get_line(int fd,int offset, char * line,int size, int line_no)
{
    int i=0;
    int current_line=0;
    int chars=0; //the number of read bytes from a section file - if it exceeds the size of the section, it meas we're out of it
    int bytes_to_read,copy_size=size;
    char * store = (char*)malloc(size*sizeof(char));
    char buf[100];

    if(copy_size>=64){
        bytes_to_read=64;
        copy_size-=64;
    }
    else{
        bytes_to_read=copy_size;
    }
    lseek(fd,offset,SEEK_SET);
    while(read(fd,buf,bytes_to_read)!=0 && current_line<line_no && copy_size>=0){
        chars=chars+bytes_to_read;
        for(int j=0;j<bytes_to_read; j++){
            store[i]=buf[j];
            if(buf[j]=='\n' || chars>size){
                current_line++;
                if(current_line<line_no){
                    i=0;
                }
                else if(current_line==line_no){
                    break;
                }
            }
            else {
                i++;
            }
        }
        if(copy_size>=64){
            bytes_to_read=64;
            copy_size-=64;
        }
        else{
            bytes_to_read=copy_size;
        }
    }
    //if the nr of lines in the section is smaller than the one we were looking for --> value returned ==-1, meaning the line was not found
    if(current_line < line_no) return -1;
    for(int j=0;j<=i;j++){
        line[j]=store[j];
    }
    line[i]='\0';
    free(store);
    return i;
}

//this function verifies if the data regarding to  a section file satisfies the conditions or not
void extract_line(char *path,struct Section sections[],char nr_section,int sec,int lin)
{
    int fd,offset,len;
    char *line;

    //the section we are looking for is available in the given file or not
    if(sec>nr_section){
        printf("ERROR\ninvalid section");
        return;
    }
    if((fd=open(path,O_RDONLY))<0){
        printf("Cannot open file");
        exit(6);
    }

    offset=sections[sec-1].offset;
    line=(char*)malloc(sections[sec-1].size*sizeof(char));
    len=get_line(fd,offset,line,sections[sec-1].size,lin);

    //if the value returned is -1, it means we didn't find the line in the section
    if(len == -1){
        printf("ERROR\ninvalid line");
        return;
    }

    //otherwise we were successful, the line is printed backwards
    printf("SUCCESS\n");
    for(int i=len-1;i>=0;i--){
        printf("%c",line[i]);
    }
    free(line);
    close(fd);

}

//this function finds all those files that satisfy the given condition: min 4 sections of type 44 -- it is similar to the recursive printing(opens each directory in the subtree)
void find_allSF(char* path)
{
    DIR *directory;
    char  title [PATH_MAX];
    struct dirent * entry;
    char nr_section;
    struct stat inode;
    struct Section sections[15];
    short version;
    int j,count=0;

    // if the path "leads to an entry, that is not a directory, it is verified whether it is a section file that we need to print or not"
    lstat(path,&inode);
    if(!S_ISDIR(inode.st_mode)){
        //calling the tester function, to tell whether is a valid sf or not,; if yes, than the data stored by its header is verified
        j = test_SF(path, sections, &nr_section, &version);
        if (j == 0){
            if(nr_section<4){
                return;
            }
            for(int i=0; i<nr_section; i++){
                if(sections[i].type==44){
                    count++;
                }
            }
            if(count>=4){
                printf("%s\n",path);
            }

        }

        return;
    }
    else{
        //if it is a directory, it is opened for further searching
        directory = opendir(path);
        while((entry = readdir(directory))!=0){
            sprintf(title,"%s/%s",path,entry->d_name);
            if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0  ) {
                find_allSF(title);
            }
        }
        closedir(directory);
    }

}


//main
int main(int argc, char **argv)
{
    struct stat inode;
    struct Section sections[15];
    char  path[PATH_MAX], filter_size[10],filter_perm[10],nr_section;
    int list, parse,extract,recursive,find_all,operation,j,path_nr;
    short version,sec,lin;

    //operations are only done when at least one argument is given
    if(argc >= 2) {

        //if argc is only 2 --> we either print the variant or nothing (error message)
        if (argc == 2) {
            if (strstr(argv[1], "variant")) {
                printf("59153\n");
            } else {
                printf("Not a valid command!\n");
                exit(0);
            }
        }
        //if argc >2 it means we have a whole sequence of arguments in any order possible -->
            // we need to traverse the whole arguments list to tell whether it is a valid instruction, if yes, than which one
        else {
            list = 0;
            parse = 0;
            extract = 0;
            recursive = 0;
            find_all = 0;
            path_nr=0;
            lin = -1;
            sec = -1;
            for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], "list") == 0) {
                    list++;
                } else if (strcmp(argv[i], "parse") == 0) {
                    parse++;
                } else if (strcmp(argv[i], "extract") == 0) {
                    extract++;
                } else if (strcmp(argv[i], "findall") == 0) {
                    find_all++;
                } else if (strcmp(argv[i], "recursive") == 0) {
                    recursive ++;
                } else if (strstr(argv[i], "path=")) {
                    strcpy(path, argv[i] + 5);
                    path_nr++;
                } else if (strstr(argv[i], "size_smaller=")) {
                    if(check_number(argv[i]+13)){
                        strcpy(filter_size,(argv[i] + 13));
                    }
                } else if (strstr(argv[i], "permissions=")) {
                    strcpy(filter_perm, argv[i] + 12);
                } else if (strstr(argv[i], "section=")) {
                    if(check_number(argv[i]+8)){
                        sec = atoi(argv[i] + 8);
                    }
                } else if (strstr(argv[i], "line=")) {
                    if(check_number(argv[i]+5)){
                        lin = atoi(argv[i] + 5);
                    }
                } else {
                    printf("Not a valid command!\n");
                    exit(1);
                }
            }

            //based on the set values, calling the input_parse function we can tell which functionality is needed (if the correct structure was given)
            operation = input_parse(list, parse, extract, find_all, recursive,path_nr, path, filter_perm, filter_size, sec, lin);
            switch (operation) {
                case -5:
                    printf("Something terribly went wrong!\n");
                    break;
                case -2:
                    printf("Not a valid command!\n");
                    break;
                case -1:
                    printf("ERROR\ninvalid directory path");
                    break;
                case 0:
                    print_content(filter_size, filter_perm, path);
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
                    break;
                case 3:
                    j = test_SF(path, sections, &nr_section, &version);
                    if (j != 0) {
                        printf("ERROR\ninvalid file");
                        printf("%d ", j);
                        exit(9);
                    }
                    extract_line(path, sections, nr_section, sec, lin);
                    break;
                case 4:
                    if (lstat(path, &inode) < 0) {
                        printf("ERROR\ninvalid directory path");
                        exit(2);
                    }
                    printf("SUCCESS\n");
                    find_allSF(path);
                default: ;

            }

        }
    }

    return 0;
}