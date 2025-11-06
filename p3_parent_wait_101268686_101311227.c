#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("[P1 parent] starting (pid=%d)\n", getpid());

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        // Child path: exec the separate child program
        execl("./p3_child", "p3_child", (char*)NULL);
        perror("execl p3_child"); _exit(127);
    }

    // Parent path: wait for child to finish
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) { perror("waitpid"); return 1; }

    if (WIFEXITED(status))
        printf("[P1 parent] child exited with code %d. Parent exiting.\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("[P1 parent] child killed by signal %d. Parent exiting.\n", WTERMSIG(status));
    else
        printf("[P1 parent] child ended (stopped/continued). Parent exiting.\n");

    return 0;
}
