#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2];
    int p2[2];
    char buf = 'x';

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        printf("Pipe error\n");
        exit(1);
    }

    int PID = fork();
    if (PID > 0) {
        close(p1[0]);
        close(p2[1]);

        write(p1[1], &buf, 1);

        if (read(p2[0], &buf, 1) == 1) {
            printf("%d: received pong\n", getpid());
        }

        close(p1[1]);
        close(p2[0]);
        wait((int *)0);
        exit(0);

    } else if (PID == 0) {
        close(p1[1]);
        close(p2[0]);

        if (read(p1[0], &buf, 1) == 1) {
            printf("%d: received ping\n", getpid());
        }

        write(p2[1], &buf, 1);

        close(p1[0]);
        close(p2[1]);
        exit(0);

    } else {
        printf("Fork error\n");
        exit(1);
    }
}