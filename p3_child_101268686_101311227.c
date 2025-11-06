#define _XOPEN_SOURCE 700
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("[P2 child] started (pid=%d)\n", getpid());
    int x = 0;
    while (x >= -500) {
        x -= 7;            // decrement step; will eventually cross -500
        usleep(20000);     // 20 ms so we can see output gradually
    }
    printf("[P2 child] reached %d (< -500). Exiting.\n", x);
    return 0;
}
