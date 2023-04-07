#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

off_t getFileSize(const char *path)
{
    int fd;
    off_t size;

    fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("Could not open input file");
        exit(EXIT_FAILURE);
    }

    size = lseek(fd, 0, SEEK_END);
    close(fd);

    return size;
}
int perm_string_to_octal(const char *perm_str)
{
    int octal = 0;

    for (int i = 0; i < strlen(perm_str); i++)
    {
        switch (perm_str[i])
        {
        case 'r':
            octal |= 4; 
            break;
        case 'w':
            octal |= 2; 
            break;
        case 'x':
            octal |= 1; 
            break;
        case '-':
        default:
            break;
        }

        if ((i + 1) % 3 == 0)
        {
            octal <<= 3;
        }
    }

    return octal;
}
int listPerm(const char *path, const char perm[],int recursive)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return 0;
    }

    struct dirent *entry = NULL;
    struct stat statbuf;
    char fullPath[512];
    mode_t permissions;
    permissions = perm_string_to_octal(perm) / 8;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {

                if (permissions == (statbuf.st_mode & 0777))
                {
                    printf("%s\n", fullPath);
                    if(recursive==1) listPerm(fullPath,perm,recursive);
                }
            }
        }
    }

    closedir(dir);
    return 1;
}

int listRec(const char *path)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return 0;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                printf("%s\n", fullPath);
                if (S_ISDIR(statbuf.st_mode))
                {
                    listRec(fullPath);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}

int listDir(const char *path, int rec, int s)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return -1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            printf("%s/%s\n", path, entry->d_name);
    }
    closedir(dir);
    return 0;
}

int listSize(const char *path, int size, int recursive)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if (dir == NULL)
    {
        perror("Could not open directory");
        return 0;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0)
            {
                if (S_ISREG(statbuf.st_mode))
                {
                    if (size < statbuf.st_size)
                        printf("%s\n", fullPath);
                }

                if (S_ISDIR(statbuf.st_mode))
                {
                    if (recursive == 1)
                        listSize(fullPath, size, recursive);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}
int parse(const char *path)
{
    int fd;
    fd = open(path, O_RDONLY);
    lseek(fd, -1 ,SEEK_END);
    char magic;
    read(fd, &magic, 1);
    if(magic!='u') {
        printf("ERROR\nwrong magic");
        return -1;
    }
    lseek(fd, -3, SEEK_END);
    short int head;
    read(fd, &head, 2);
    lseek(fd,0, SEEK_END);
    lseek(fd,-head, SEEK_END);
    char buffer[5];
    read(fd, buffer, sizeof(buffer));
    int no_of_section = buffer[4];
    if(no_of_section<6 || no_of_section>16) {
        printf("ERROR\nwrong sect_nr\n");
        return -1;
    }
    int version = buffer[0];
    if(version<26 || version>110) {
        printf("ERROR\nwrong version\n");
        return -1;
    }
    

    int k = 0;
    
    char section[29];
    

     while(k < no_of_section)
    {
        char sect_name[18];
        int sect_type, sect_offset, sect_size;
        read(fd,section,29);
        memcpy(sect_name, section, 17);
        sect_name[17] = '\0';
        memcpy(&sect_type, section+17, 4);
        memcpy(&sect_offset, section+21, 4);
        memcpy(&sect_size, section+25, 4);
        section[29]='\0';
         if (sect_type!=98 && sect_type!=96 && sect_type!=50) {
            printf("ERROR\nwrong sect_types\n");
            
            return -1;
        }

       // printf("section%d: %s %d %d\n",k+1,sect_name,sect_type,sect_size);          
        k++;
    }
    lseek(fd,-29*no_of_section,SEEK_CUR);
    k=0;
      printf("SUCCESS\n");
      printf("version=%d\n",version);
      printf("nr_sections=%d\n",no_of_section);
    //   k=0;
    while(k < no_of_section)
    {
        char sect_name[18];
        int sect_type, sect_offset, sect_size;
        read(fd,section,29);
        memcpy(sect_name, section, 17);
        sect_name[17] = '\0';
        memcpy(&sect_type, section+17, 4);
        memcpy(&sect_offset, section+21, 4);
        memcpy(&sect_size, section+25, 4);
        section[29]='\0';
        
         if (sect_type!=98 && sect_type!=96 && sect_type!=50) {
         
            return -1;
        }

         printf("section%d: %s %d %d\n",k+1,sect_name,sect_type,sect_size);          
        k++;
    }
   
    close(fd);
    return 0;
}

int main(int argc, char **argv)
{
    char path[10000];
    char perm[10] = "";
    int size = 0;
    int recursive;
    int permision = 0;
    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
            printf("69227\n");
        else if (strcmp(argv[1], "list") == 0)
        {
            size = 0;
            recursive = 0;
            printf("SUCCESS\n");
            for (int i = 2; i < argc; i++)
            {
                if (strstr(argv[i], "path="))
                {
                    strcpy(path, argv[i] + 5);
                }
                if (strstr(argv[i], "recursive"))
                {
                    recursive = 1;
                }
                if (strstr(argv[i], "size_greater="))
                {
                    size = atoi(argv[i] + 13);
                }
                if (strstr(argv[i], "permissions="))
                {
                    strcpy(perm, argv[i] + 12);
                    permision = 1;
                }
            }

            if (size != 0)
            {
                listSize(path, size, recursive);
            }
            else
            {
                if (permision == 1)
                {
                    listPerm(path, perm,recursive);
                }
                else
                {
                    if (recursive == 1)
                    {
                        listRec(path);
                    }
                    else
                    {
                        listDir(path, recursive, size);
                    }
                }
            }
        } else if (strcmp(argv[1], "parse") == 0)
        {
            
            parse(argv[2]+5);
        }
    }
    return 0;
}
