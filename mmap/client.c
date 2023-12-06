#include "shared.h"
#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void fail(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
void *fmem;
void handler(int sig) {
  if (sig == SIGINT) {
    printf("\rInterrupt\n");
  }

  munmap(fmem, MEM_SIZE);
  exit(sig);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fail("usage: ./server.out <file_name>");
  }

  if (signal(SIGINT, &handler) == SIG_ERR) {
    fail("signal");
  }

  int fd;

  if ((fd = open(argv[1], O_RDWR)) < 0) {
    fail("shm_open");
  }

  if ((fmem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
      MAP_FAILED) {
    fail("mmap");
  }

  close(fd);

  char message[MEM_SIZE];

  short len = 0;
  memcpy(&len, fmem, sizeof(short));
  short offset = len;

  printf("offset = %d\n", offset);
  printf("Enter a message: ");
  fgets(message, MEM_SIZE - offset, stdin);
  memcpy(fmem + offset, message, strlen(message) - 1);
  msync(fmem, MEM_SIZE, MS_SYNC | MS_INVALIDATE);

  handler(0);
}
