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
                        listSize(fullPath,size,recursive);
                }
            }
        }
    }
    closedir(dir);
    return 0;
}
int main(int argc, char **argv)
{
    char path[10000];
    int size = 0;
    int recursive;
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
            }
            if (size != 0)
        {
            listSize(path, size, recursive);
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
        
    }
    return 0;
}
