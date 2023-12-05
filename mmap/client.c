#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MEM_SIZE 1024

int main() {
  int fd;
  void *fmem;
  fd = open("mem", O_RDWR, S_IRUSR | S_IWUSR);
  fmem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  munmap(fmem, MEM_SIZE);

  return 0;
}
