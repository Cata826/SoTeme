#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "a2_helper.h"
#include <pthread.h>
#include <semaphore.h>
int ok=0;
int ok8=0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
sem_t sem;
sem_t *semaphore;
sem_t *semaphore2;
void *thread_function(void *arg)
{
    int thread_num = *((int *)arg);
    pthread_mutex_lock(&lock);
    if(thread_num==1)
    {
        if(ok==1)
            info(BEGIN,2,thread_num);
        else
        {
            pthread_cond_wait(&cond,&lock);
            info(BEGIN, 2, thread_num);
        }
    }
    else if(thread_num==2)
    {
            sem_wait(semaphore);
            info(BEGIN, 2, thread_num);
    }
    else if(thread_num==3)
    {
            info(BEGIN, 2, thread_num);
            pthread_cond_signal(&cond);
            ok=1;
    }else 
         info(BEGIN, 2, thread_num);
    if(thread_num==3)
    {
        pthread_cond_wait(&cond1,&lock);
        info(END, 2, thread_num);
    }
    else if(thread_num==2)
    {
          info(END, 2, thread_num);
          sem_post(semaphore2);
         
    }
    else if(thread_num==1)
    { 
        info(END, 2, thread_num);
        pthread_cond_signal(&cond1);
    }
    else
       info(END, 2, thread_num);
        
        
    pthread_mutex_unlock(&lock);
    return NULL;
}
void *thread_function8(void *arg)
{
    int thread_num = *((int *)arg);
    if(thread_num==1)
    {  
        sem_wait(semaphore2);
        info(BEGIN, 8, thread_num);
    }
    else
        info(BEGIN, 8, thread_num);
    if (thread_num == 4) {
      
        info(END, 8, thread_num);
          sem_post(semaphore); 
        ok8=1;
     }
     else 
     info(END, 8, thread_num);
    return NULL;
}
void *thread_function6(void *arg)
{
    int thread_num = *((int *)arg);
    sem_wait(&sem);
    info(BEGIN, 6, thread_num);

    info(END, 6, thread_num);
    sem_post(&sem);
   
    
    return NULL;
}
int main(void)
{
    init();
    info(BEGIN, 1, 0);
    semaphore = sem_open("/my_semaphore", O_CREAT, 0644, 0);
     semaphore2 = sem_open("/my_semaphore2", O_CREAT, 0644, 0);
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock1, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond1,NULL);
    pid_t pid2, pid3, pid4, pid5, pid6, pid7, pid8, pid9;
    

    pid2 = fork();
    if (pid2 == -1)
    {
        return -1;
    }
    else if (pid2 == 0)
    {
        info(BEGIN, 2, 0);
        pthread_t threads[4];
        int thread_args[4];

        for (int i = 0; i < 4; i++)
        {
            thread_args[i] = i + 1;
            pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        }
        for (int i = 0; i < 4; i++)
        {
            pthread_join(threads[i], NULL);
        }

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
            pthread_t threads2[4];
            int thread_args2[4];

            for (int i = 0; i < 4; i++)
            {
                thread_args2[i] = i + 1;
                pthread_create(&threads2[i], NULL, thread_function8, &thread_args2[i]);
            }
            for (int i = 0; i < 4; i++)
            {
                pthread_join(threads2[i], NULL);
            }
            // printf("[CHILD 2] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());

            info(END, 8, 0);
            return 0;
        }

        waitpid(pid8, NULL, 0);
        waitpid(pid4, NULL, 0);
        info(END, 4, 0);

        return 0;
    }
    sem_close(semaphore);
    sem_unlink("/my_semaphore");
    sem_close(semaphore2);
    sem_unlink("/my_semaphore2");

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
        sem_init(&sem, 0, 4);
        info(BEGIN, 6, 0);
        pthread_t threads1[50];
        int thread_args1[50];
        for (int i = 0; i < 50; i++)
        {
            thread_args1[i] = i + 1;
            pthread_create(&threads1[i], NULL, thread_function6, &thread_args1[i]);
        }

        for (int i = 0; i < 50; i++)
        {
            pthread_join(threads1[i], NULL);
        }

        // printf("[CHILD 6] My PID is %d. My parent's PID is %d.\n", getpid(), getppid())
        // info(END, 6, 11);
        info(END, 6, 0);
        sem_destroy(&sem);
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