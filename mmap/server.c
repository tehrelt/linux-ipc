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

int main() {
  int fd;
  void *fmem;
  fd = open("mem", O_RDWR, S_IRUSR | S_IWUSR);

  fmem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  int frame = 0;
  int outdate = 0;
  time_t mtime = *(time_t *)fmem;
  while (1) {

    if (mtime != *(time_t *)fmem) {
      outdate = 1;
      mtime = *(time_t *)fmem;
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
