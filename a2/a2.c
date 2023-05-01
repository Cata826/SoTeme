#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "a2_helper.h"

int main(void)
{
    init();
    info(BEGIN, 1, 0);

    pid_t pid2, pid3, pid4, pid5, pid6, pid7, pid8, pid9;

    pid2 = fork();
    if (pid2 == -1)
    {
        return -1;
    }
    else if (pid2 == 0)
    {
        info(BEGIN, 2, 0);
        // printf("[CHILD 2] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
        info(END, 2, 0);
        return 0;
    }

    pid3 = fork();
    if (pid3 == -1)
    {
        return -1;
    }
    else if (pid3 == 0)
    {
        info(BEGIN, 3, 0);
        // printf("[CHILD 3] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());

        pid7 = fork();
        if (pid7 == -1)
        {
            return -1;
        }
        else if (pid7 == 0)
        {
            info(BEGIN, 7, 0);
            // printf("[GRANDCHILD 7] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
            info(END, 7, 0);
            return 0;
        }
 waitpid(pid7, NULL, 0);
         waitpid(pid3, NULL, 0);
        info(END, 3, 0);
       

        return 0;
    }

    pid4 = fork();
    if (pid4 == -1)
    {
        return -1;
    }
    else if (pid4 == 0)
    {
        info(BEGIN, 4, 0);
        // printf("[CHILD 4] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());

        pid8 = fork();
        if (pid8 == -1)
        {
            return -1;
        }
        else if (pid8 == 0)
        {
            info(BEGIN, 8, 0);
            // printf("[GRANDCHILD 8] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
            info(END, 8, 0);
            return 0;
        }

        waitpid(pid8, NULL, 0);
         waitpid(pid4, NULL, 0);
        info(END, 4, 0);

    

        return 0;
    }

    pid5 = fork();
    if (pid5 == -1)
    {
        return -1;
    }
    else if (pid5 == 0)
    {
        info(BEGIN, 5, 0);
        // printf("[CHILD 5] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
        info(END, 5, 0);
        return 0;
    }

    pid6 = fork();
    if (pid6 == -1)
    {
        return -1;
    }
    else if (pid6 == 0)
    {
        info(BEGIN, 6, 0);
        // printf("[CHILD 6] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
        info(END, 6, 0);
        return 0;
    }

    pid9 = fork();
    if (pid9 == -1)
    {
        return -1;
    }
    else if (pid9 == 0)
    {
        info(BEGIN, 9, 0);
        // printf("[GRANDCHILD 9] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
        info(END, 9, 0);
        return 0;
    }

    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
    waitpid(pid4, NULL, 0);
    waitpid(pid5, NULL, 0);
    waitpid(pid6, NULL, 0);
    waitpid(pid9, NULL, 0);

    info(END, 1, 0);
    return 0;
}
