#ifndef SHARED_H
#define SHARED_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#define SHARED_MEM_NAME "goddamn"

#define SEM_SERVER 1337

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};

int sem_i(int semid) {
  union semun semu;
  semu.val = 1;
  if (semctl(semid, 0, SETVAL, semu) == -1) {
    perror("semctl");
    return -1;
  }
  return 0;
}

int sem_dec(int semid) {
  struct sembuf semb;
  semb.sem_num = 0;
  semb.sem_op = -1;
  semb.sem_flg = SEM_UNDO;

  if (semop(semid, &semb, 1) == -1) {
    fprintf(stderr, "sem_dec failed\n");
    return -1;
  }

  return 0;
}

int sem_inc(int semid) {
  struct sembuf semb;
  semb.sem_num = 0;
  semb.sem_op = 1;
  semb.sem_flg = SEM_UNDO;

  if (semop(semid, &semb, 1) == -1) {
    fprintf(stderr, "sem_dec failed\n");
    return -1;
  }

  return 0;
}
#endif // ! SHARED_H
