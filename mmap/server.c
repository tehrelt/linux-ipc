#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MEM_SIZE 4096

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stdout, "server: enter a file arg\n");
    exit(EXIT_FAILURE);
  }

  int fd;
  void *fmem;
  if ((fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if ((fmem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
      MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  int frame = 0;
  int outdate = 0;
  struct stat statbuf;
  if (fstat(fd, &statbuf) == -1) {
    perror("fstat");
    exit(EXIT_FAILURE);
  };
  struct timespec mtime = statbuf.st_mtim;

  while (1) {
    if (fstat(fd, &statbuf) == -1) {
      perror("fstat");
      exit(EXIT_FAILURE);
    }

    if (statbuf.st_mtim.tv_nsec != mtime.tv_nsec) {
      outdate = 1;
      mtime = statbuf.st_mtim;
    }

    if (outdate) {
      printf("Frame: %d\n", frame);
      printf("%s\n", fmem);
      sleep(1);
      system("clear");
      frame++;
      outdate = 0;
    }
  }

  close(fd);
  munmap(fmem, MEM_SIZE);

  return 0;
}
