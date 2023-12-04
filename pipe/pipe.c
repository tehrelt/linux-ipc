#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LENGTH 256

int main() {
  int fd[2]; // fd[0] - read  fd[1] - write
  pid_t pid;

  pipe(fd);
  pid = fork();

  if (pid == -1) {
    perror("cannot fork a process");
    exit(errno);
  }

  if (pid == 0) { // fork-process
    close(fd[0]); // closing read fd

    char buf[MAX_LENGTH];

    printf("Enter a message: ");
    scanf("%s", buf);

    FILE *file = fopen("output.txt", "a");
    if (file == NULL) {
      printf("cannot open file\n");
      exit(-1);
    }

    fwrite(buf, strlen(buf), 1, file);
    fwrite("\n", strlen("\n"), 1, file);
    write(fd[1], buf, strlen(buf) + 1); // writing to fd[1]

    fclose(file);

    exit(0);
  } else {        // main-process
    close(fd[1]); // closing write fd

    char buf[MAX_LENGTH];

    read(fd[0], buf, sizeof(buf));       // reading from fd[0]
    printf("Written string: %s\n", buf); // printing result
  }

  return 0;
}
