#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MEM_SIZE 4096

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stdout, "server: enter a file arg\n");
    exit(EXIT_FAILURE);
  }

  int fd;
  void *fmem;

  char message[MEM_SIZE];

  if ((fd = open(argv[1], O_WRONLY | O_EXCL | O_CREAT)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if ((fmem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
      MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  fgets(message, MEM_SIZE, stdin);

  memcpy(message, fmem, MEM_SIZE);

  return 0;
}
