#include "shared.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char *pipe_name = 0;

void handler(int sig) {
  if (sig == SIGPIPE) {
    printf("Client disconnected\n");
  } else if (sig == SIGINT) {
    printf("Interrupted by user\n");
  } else if (sig == 0) {
    printf("Server closed\n");
  }

  char buf[MAX_LEN];
  sprintf(buf, "rm %s", pipe_name);
  printf("Removing '%s'\n", pipe_name);
  system(buf);
  exit(0);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    printf("Usage: ./server.out pipe_name\n");
    exit(-1);
  }

  if (signal(SIGPIPE, &handler) == SIG_ERR) {
    perror("cannot bind handler to SIGPIPE\n");
    exit(-1);
  }

  if (signal(SIGINT, &handler) == SIG_ERR) {
    perror("cannot bind handler to SIGINT\n");
    exit(-1);
  }

  pipe_name = argv[1];

  if (mkfifo(pipe_name, 0777) == -1) {
    if (errno != EEXIST) {
      printf("cant create fifo\n");
      return 1;
    }
  }

  int first_msg = 1;

  while (1) {

    if (first_msg) {
      printf("Waiting for client\n");
    }

    int wrfd = open(pipe_name, O_WRONLY);
    if (wrfd == -1) {
      printf("Cannot open a FIFO\n");
      return 2;
    }

    if (first_msg) {
      printf("Connection established\n");
      first_msg = 0;
    }

    char message[MAX_LEN];

    printf("Enter a message: ");
    fgets(message, MAX_LEN, stdin);
    write(wrfd, message, strlen(message) + 1);
  }
  handler(0);
  return 0;
}
