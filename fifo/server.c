#include "shared.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  if (mkfifo(FIFO_NAME, 0777) == -1) {
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

    int wrfd = open(FIFO_NAME, O_WRONLY);
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
    if (write(wrfd, message, strlen(message) + 1) == -1) {
      printf("Client disconnected\n");
    };
  }
  return 0;
}
