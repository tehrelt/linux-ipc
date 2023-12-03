#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd[2];
  pid_t pid;
  char str[] = "char\n";
  char buf[80];

  pipe(fd);
  pid = fork();

  if (pid == -1) {
    return -1;
  } else if (pid == 0) { // fork-process
    close(fd[0]);

    write(fd[1], str, strlen(str) + 1);
    exit(0);

  } else { // current-process
    close(fd[1]);

    read(fd[0], buf, sizeof(buf));

    printf("Recieved string: %s", buf);

    exit(0);
  }

  return 0;
}
