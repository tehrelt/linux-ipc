#include "shared.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void fail(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void *mmfp;

void handler(int sig) {
  if (sig == SIGINT) {
    printf("\rInterrupt\n");
  }

  munmap(mmfp, MEM_SIZE);
  exit(sig);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fail("usage: ./server.out <file_name>");
  }

  if (signal(SIGINT, &handler) == SIG_ERR) {
    fail("signal");
  }

  int shmid;
  int fd;
  int frame = 0;

  if ((fd = open(argv[1], O_CREAT | O_RDWR, 0666)) == -1) {
    fail("shm_open");
  }

  if (ftruncate(fd, MEM_SIZE) == -1) {
    fail("ftruncate");
  }

  if ((mmfp = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
      MAP_FAILED) {
    fail("mmap");
  }

  short len = 0;

  close(fd);

  while (1) {
    len = strlen(mmfp + sizeof(short)) + sizeof(short);
    memcpy(mmfp, &len, sizeof(short));

    if (len == 4096) {
      printf("frame overflow\n");
      handler(1);
    }

    printf("Frame %d, offset %d:\n%s\n\n", frame, len,
           (char *)mmfp + sizeof(short));
    frame++;
    sleep(1);
  }
}
