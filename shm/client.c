#include "shared.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define U1 1
#define U2 2

#define SHM_KEY 1337
#define SHM_SIZE 1024
#define CH_WAIT 10
#define CH_CONNECTED 11
#define CH_DISCONNECTED 12
#define CH_TYPING 13

#define OFFSET 1 + 30

int shmid = -12;
char *shm;

void handler(int sig) {
  if (shmid != -1 && *shm == CH_DISCONNECTED) {
    printf("Removing shmid=%d\n", shmid);
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
      perror("shmctl");
      exit(1);
    }
  }

  if (sig == SIGINT)
    printf("\r\rInterrupt by user\n");

  *shm = CH_DISCONNECTED;
  exit(1);
}

int main() {

  printf("guide: press ctrl+c to interrupt\n");

  char username[30];
  int turn = 0;
  char *s;

  if (signal(SIGINT, &handler) == SIG_ERR) {
    perror("signal");
    exit(-1);
  }

  printf("Enter a username: ");
  fgets(username, 30, stdin);

  // Create a segment of shared memory
  if ((shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666)) < 0) {
    if (shmid == -1) {
      if ((shmid = shmget(SHM_KEY, SHM_SIZE, 0666)) < 0) {
        perror("shmget[opponent]");
        exit(-1);
      }

      turn = 1;
    }
  } else {
    printf("Shared memory created [shmid=%d]\n", shmid);
  }

  // Attach to a shared memory
  if ((shm = shmat(shmid, NULL, 0)) == (char *)-1) {
    perror("shmat: error occured\n");
    exit(-1);
  }

  printf("Shared memory attached [shmid=%d][turn=%d]\n", shmid, turn);

  if (turn == 1) {
    *shm = CH_CONNECTED;
    printf("shm = CH_CONNECTED\n");
  } else if (turn == 0) {
    fprintf(stdout, "Waiting opponent");
    *shm = CH_WAIT;
  }

  while (*shm == CH_WAIT) {
    fflush(stdout);
    fprintf(stdout, ".");
    sleep(3);
  }

  printf("\n");

  if (turn == 0 && *shm == CH_CONNECTED) {
    printf("Opponent connected\n");
  }
  s = shm;

  while (1) {

    if (turn) {
      char message[SHM_SIZE];
      *shm = CH_TYPING;
      printf("Enter message: ");
      fgets(message, SHM_SIZE + ((OFFSET) * (-1)), stdin);
      s = shm + OFFSET;

      memcpy(shm + 1, username, strlen(username) - 1);

      int i = 0;
      int lenU = strlen(username) - 1;
      for (i = 0; i < lenU; i++) {
        *(shm + 1 + i) = username[i];
      }
      *(shm + 1 + i) = '\0';

      int len = strlen(message) - 1;
      for (i = 0; i < len; i++) {
        *s++ = message[i];
      }
      *s++ = '\0';

      if (*shm == CH_DISCONNECTED) {
        break;
      }

      turn = 0;
      *shm = CH_WAIT;

      while (*shm == CH_WAIT)
        ;

    } else {
      printf("Opponent is typing");
      int n = 0;
      while (*shm == CH_TYPING) {
        fflush(stdout);
        printf(".");
        if (n == 3) {
          n = 0;
          printf("\b\b\b   \b\b\b");
        }
        n++;
        sleep(1);
      }

      printf("\n");

      printf("%s: %s", shm + 1, shm + OFFSET);
      printf("\n");

      if (*shm == CH_DISCONNECTED) {
        break;
      }
      turn = 1;
    }
  }

  handler(0);
}
