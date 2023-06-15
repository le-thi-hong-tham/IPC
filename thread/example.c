#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
pthread_mutex_t mutex;

void recursive_function(int count) {
    pthread_mutex_lock(&mutex); // Acquire the mutex

    if (count > 0) {
        printf("Recursive call: %d\n", count);
        recursive_function(count - 1);
    }

    pthread_mutex_unlock(&mutex); // Release the mutex
}

int main() {
    pid_t pid;
    // Fork a child process
    pid = fork();
    if (pid < 0) 
    {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } 
    else if (pid == 0) 
    {
        printf ("I am a child and my pid = %d\n", getpid());
        // Execute a different program using exec
        char *args[] = {"./semaphore", NULL};  // Example command: ls -l
        execvp(args[0], args);
        // execvp will only return if there's an error
        fprintf(stderr, "Exec failed\n");
        return 1;
    } 
    else 
    {
        // Parent process
        printf ("I am a parent and my pid = %d\n", getpid());
        pthread_t thread;

        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mutex, &attr); // Initialize the recursive mutex

        recursive_function(5);

        pthread_mutex_destroy(&mutex); // Destroy the mutex
        wait(NULL);
        printf("Child process finished\n");
        exit(0);
    }
}