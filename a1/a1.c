#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
int esteReg(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char filePath[512];
    struct stat statbuf;
	int ok=0;
    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return -1;
    }
    while((entry = readdir(dir)) != NULL) {
        snprintf(filePath, 512, "%s/%s", path, entry->d_name);
        if(lstat(filePath, &statbuf) == 0) {
            if(S_ISREG(statbuf.st_mode)) {
			ok=1;
               }
        }
    }        
    closedir(dir);
    return ok;

    }


off_t getFileSize(const char* path) {
    int fd;
    off_t size;

    fd = open(path, O_RDONLY);
    if(fd == -1) {
        perror("Could not open input file");
        exit(EXIT_FAILURE);
    }

    size = lseek(fd, 0, SEEK_END);
    //printf("The file size is %ld.\n", size);
    close(fd);

    return size;
}
int listRec(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return 0;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0) {
                printf("%s\n", fullPath);
                if(S_ISDIR(statbuf.st_mode)) {
                    listRec(fullPath);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}

int listDir(const char *path) {
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    
    dir = opendir(path);
    if(dir == NULL) {
        perror("Could not open directory");
        return -1;
    }
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
        printf("%s/%s\n",path,entry->d_name);
    }
    closedir(dir);
    return 0;
}

int main(int argc, char **argv){
    if(argc >= 2)
    {
        if(strcmp(argv[1], "variant") == 0){
            printf("69227\n");
        }
        
        if(strcmp(argv[1], "list") == 0)
        {            
            
                if(strstr(argv[2], "recursive"))
                { 
                if(strstr(argv[3],"path="))
                {
                // char size[10000];
                // sscanf(argv[3],"%s",size);
                // char *token;int number=0;
                // token = strtok(size, "=");
                // token = strtok(NULL, "=");
                // number=atoi(token);
                // }
                printf("SUCCESS\n");
                char path[10000];
                sscanf(argv[3],"%s",path);
                char *tok;
                tok = strtok(path, "=");
                tok = strtok(NULL, "=");
                listRec(tok);
                //  if(strstr(argv[4],"size_greater="))
                //  {
                // printf("SUCCESS\n");
                // char path[10000];
                // sscanf(argv[3],"%s",path);
                // char *tok;
                // tok = strtok(path, "=");
                // tok = strtok(NULL, "=");
                // //esteReg(tok);
               // listRec(tok);
                // }
               
                // listRec(tok);
                // }
                
                }else printf("ERROR\ninvalid directory path\n");
                } else
                
                if(strstr(argv[2],"path="))
                {
                printf("SUCCESS\n");
                char path[10000];
                sscanf(argv[2],"%s",path);
                char *tok;
                tok = strtok(path, "=");
                tok = strtok(NULL, "=");
                listDir(tok);
                }
                else printf("ERROR\ninvalid directory path\n");
                    
          }
        }              
    return 0;
}