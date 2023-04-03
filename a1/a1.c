#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

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
        if(strcmp(argv[1], "list") == 0){
            printf("SUCCESS\n");
            char path[10000];
            //const char *paths = "Desktop/temaSO/SoTeme/a1";
            //char *path="test_root/2tQG4/a5tHxd/XEmLvY/jxD3EphA.Nht";
            sscanf(argv[2],"%s",path);
            char *tok;
            tok = strtok(path, "=");
            tok = strtok(NULL, "=");
            //char path[10000];
            listDir(tok);
            // char *path1=""
           
        }    
    }
    return 0;
}
