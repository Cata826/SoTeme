#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
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
                // if(strcmp(argv[2], "recursive")==0)
                // {
                //     printf("SUCCESS\n");
                //     char path[10000];
                //     sscanf(argv[2],"%s",path);
                //     char *tok;
                //     tok = strtok(path, "=");
                //     tok = strtok(NULL, "=");
                //     listRec(tok);                    
                // }
                if(strcmp(argv[2], "recursive")==0)
                {
                printf("SUCCESS\n");
                char path[10000];
                sscanf(argv[3],"%s",path);
                char *tok;
                tok = strtok(path, "=");
                tok = strtok(NULL, "=");
                listRec(tok);
                } else //if(strcmp(argv[2], "simple")==0)
                {
                printf("SUCCESS\n");
                char path[10000];
                sscanf(argv[2],"%s",path);
                char *tok;
                tok = strtok(path, "=");
                tok = strtok(NULL, "=");
                listDir(tok);
                }
               
        }  
            
        }    
           
    return 0;
}
