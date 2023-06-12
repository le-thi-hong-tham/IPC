#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    // Fork a child process
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf ("I am a child and my pid = %d\n", getpid());
        // printf ("My parent has pid = %d\n", getppid());
        // Execute a different program using exec
        char *args[] = {"ls", "-l", NULL};  // Example command: ls -l
        execvp(args[0], args);
        // execvp will only return if there's an error
        fprintf(stderr, "Exec failed\n");
        return 1;
    } else {
        // Parent process
        printf ("I am a parent and my pid = %d\n", getpid());
        // Wait for the child process to finish
        wait(NULL);
        printf("Child process finished\n");
        // Exit the parent process
        exit(0);
    }
}