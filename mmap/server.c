#include "shared.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MEM_SIZE 4096

int fd;
void *fmem;
int semid;

void handler(int sig) {
  if (sig == SIGINT) {
    printf("\rinterrupt\n");
  }

  if (semctl(semid, 1, IPC_RMID) < 0) {
    perror("semctl[IPC_RMID]");
  }

  exit(sig);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stdout, "server: enter a file arg\n");
    exit(EXIT_FAILURE);
  }

  if (signal(SIGINT, &handler) == -1) {
    perror("signal");
    exit(EXIT_FAILURE);
  }

  if ((semid = semget(SEM_SERVER, 1, IPC_CREAT | 0666)) == -1) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  if ((semid = semget(SEM_SERVER, 1, IPC_CREAT | 0660)) < 0) {
    perror("semget");
    exit(EXIT_FAILURE);
  }

  if ((fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fd, MEM_SIZE) == -1) {
    perror("ftruncate");
    exit(EXIT_FAILURE);
  }

  if ((fmem = mmap(0, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
      MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  int frame = 0;
  int outdate = 0;

  while (1) {

    if (outdate) {
      printf("Frame: %d\n", frame);
      printf("%s\n", fmem);
      sleep(1);
      // system("clear");
      frame++;
      outdate = 0;
    }
  }

  close(fd);
  munmap(fmem, MEM_SIZE);

  handler(EXIT_SUCCESS);
}
