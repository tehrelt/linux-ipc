#include "shared.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void interrupt(int sig) {
  printf("\r\rINTERRUPTED BY USER\n");
  exit(1);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("Usage: ./client.out pipe_name\n");
    exit(-1);
  }

  printf("guide: ctrl+c for interrupt\n\n");

  if (signal(SIGINT, &interrupt) == SIG_ERR) {
    perror("cannot bind handler to interrupt signal\n");
    exit(-1);
  }

  char *pipe = argv[1];
  int first_msg = 1;

  while (1) {

    if (first_msg) {
      printf("Connecting to server...\n");
    }

    int fd = open(pipe, O_RDONLY);
    if (fd == -1) {
      printf("Cannot open a fifo\n");
      return 1;
    }

    char buf[MAX_LEN];

    printf("Waiting for message...\n");
    read(fd, buf, MAX_LEN);

    if (first_msg) {
      printf("Connection established\n");
      first_msg = 0;
    }

    printf("Received message: %s\n", buf);
  }
}
