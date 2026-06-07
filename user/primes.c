#include "kernel/types.h"
#include "user/user.h"

void prime_generator(int) __attribute__((noreturn));

void prime_generator(int fd) {
    int prime;
    if (read(fd, &prime, 4) != 4) {
        close(fd);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p[2];
    pipe(p);

    int pid = fork();
    if (pid > 0) {        // parent
        close(p[0]);
        int x;
        while (read(fd, &x, 4) == 4) {
            if (x % prime != 0)
                write(p[1], &x, 4);
        }
        close(fd);
        close(p[1]);
        wait(0);
        exit(0);
    } else if (pid == 0) { // child
        close(p[1]);
        close(fd);
        prime_generator(p[0]);
    } else {
        printf("fork error\n");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);

    int pid = fork();
    if (pid > 0) {
        close(p[0]);
        for (int i = 2; i <= 280; i++)
            write(p[1], &i, 4);
        close(p[1]);
        wait(0);
    } else if (pid == 0) {
        close(p[1]);
        prime_generator(p[0]);
    }
    exit(0);
}