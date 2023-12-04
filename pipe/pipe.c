#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_LENGTH 256

void debug(const char *message) { printf("[DEBUG] %s", message); }

int main() {
  int fd[2]; // fd[0] - read  fd[1] - write
  pid_t pid, cpid;

  char answer;

  do {
    pipe(fd);
    pid = fork(); // returns 0 to the child proccess and pid of child process to
                  // the parent

    cpid = getpid();

    if (pid == -1) {
      perror("cannot fork a process");
      exit(errno);
    }

    if (pid == 0) { // fork-process

      close(fd[0]); // closing read fd

      char buf[MAX_LENGTH];

      printf("Enter a message: ");
      scanf("%s", buf);

      printf("[DEBUG][pid=%d] writing to fd[1]: %s\n", cpid, buf);
      write(fd[1], buf, strlen(buf) + 1); // writing to fd[1]

      exit(0);

    } else {        // main-process
      close(fd[1]); // closing write fd

      char buf[MAX_LENGTH];

      read(fd[0], buf, sizeof(buf)); // reading from fd[0]
      printf("[DEBUG][pid=%d] read from fork-process %s\n", cpid, buf);

      printf("[DEBUG][pid=%d] opening file output.txt with append mode\n",
             cpid);
      FILE *file = fopen("output.txt", "a");
      if (file == NULL) {
        printf("cannot open file\n");
        exit(-1);
      }
      printf("[DEBUG][pid=%d] file successfuly opened\n", cpid);
      fwrite(buf, strlen(buf), 1, file);
      fwrite("\n", strlen("\n"), 1, file);
      fclose(file);
      printf("[DEBUG][pid=%d] file output.txt closed\n", cpid);
      printf("Written string: %s\n", buf); // printing result
    }

    do {
      printf("[pid=%d]Want to enter one more message? (y/n) ", cpid);

      fflush(stdin);

      // scanf("%c", &answer);
      answer = getchar();
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;

      fflush(stdin);

      if (answer != 'y' && answer != 'n') {
        printf("Error! Press y or n\n");
      }
    } while (answer != 'y' && answer != 'n');
  } while (answer != 'n');

  return 0;
}
