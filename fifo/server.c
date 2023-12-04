#include "shared.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
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

  int wrfd = open(FIFO_NAME, O_WRONLY);

  if (wrfd == -1) {
    printf("Cannot open a FIFO\n");
    return 2;
  }

  int x = 0;

  printf("Enter a x: ");
  scanf("%d", &x);

  write(wrfd, &x, sizeof(int));

  return 0;
}
