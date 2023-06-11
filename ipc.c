// C++ program to demonstrate creating processes using fork()
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "tlpi_hdr.h"
#define BUF_SIZE 100
/* message structure */
struct message {
    long mtype;
    char mtext[8192];
};

int main()
{
    int pfd[2];
    char buf[BUF_SIZE];
    static int numRead = 0;
    char *message = "Hello, World! My name is Tham";
    if (pipe(pfd) == -1)
    {
        errExit("pipe");
    }

    /* create message queue */
    int msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (msqid == -1) {
        perror("msgget");

        return EXIT_FAILURE;
    }

	int n1 = fork();
	int n2 = fork();

	if (n1 > 0 && n2 > 0) {
        if (close(pfd[0]) == -1)    /* Read end is unused */
        {
            errExit("close - parent");
        }
        if (write(pfd[1], message, strlen(message)) != strlen(message))
        {
            fatal("parent - partial/failed write");
        }
        if (close(pfd[1]) == -1)
        {
            errExit("close");
        }
        wait(NULL);     /* Wait for child to finish */
        wait(NULL); 
        exit(EXIT_SUCCESS);
	}
	else if (n1 == 0 && n2 > 0)
	{
        if (close(pfd[1]) == -1)    /* Write end is unused */
        {
            errExit("close - child");
        }
		numRead = read(pfd[0], buf, BUF_SIZE);
        if (numRead == -1)
        {
            errExit("read");
        }
        if (numRead == 0)
        {
            printf("End-of-file\n");
        }
        printf("=====IPC using pipe=====\n");
        printf("read %d bytes from the pipe: %s\n", numRead, buf);
        if (close(pfd[0]) == -1)
        {
            errExit("close");
        }
        wait(NULL);
        _exit(EXIT_SUCCESS);
	}
	else if (n1 > 0 && n2 == 0)
	{
		struct message message;
        message.mtype = 23;
        memset(&(message.mtext), 0, 8192 * sizeof(char));
        (void)strcpy(message.mtext, "Hello parent!");
        /* send message to queue */
        if (msgsnd(msqid, &message, sizeof(long) + (strlen(message.mtext) * sizeof(char)) + 1, 0) == -1) {
            perror("msgsnd");
            return EXIT_FAILURE;
        }
	}
	else {

        /* receive message from queue */
        struct message message;
        if (msgrcv(msqid, &message, 8192, 0, 0) == -1) {
            perror("msgrcv");

            return EXIT_FAILURE;
        }
        printf("=====IPC using Message Queues=====\n");
        printf("%s\n", message.mtext);

        /* destroy message queue */
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("msgctl");
            return EXIT_FAILURE;
        }
    }
	return 0;
}
