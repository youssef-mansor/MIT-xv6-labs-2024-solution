#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

static char marker[] = "my very very very secret pw is: ";

int
main(int argc, char *argv[])
{
  for(int tries = 0; tries < 1024; tries++) {
    char *p = sbrk(PGSIZE);
    if(p == (char *)-1)
      exit(1);

    int ok = 1;
    for(int j = 8; j < 32; j++) {
      if(p[j] != marker[j]) {
        ok = 0;
        break;
      }
    }
    if(ok) {
      write(2, p + 32, 8);
      exit(0);
    }
  }

  exit(1);
}
