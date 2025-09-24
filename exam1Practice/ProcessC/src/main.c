#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#define BLOCK_SIZE sizeof(float) * 25

bool isTrue = false;

void SSR_SIGUSR1(int signum) { isTrue = true; }
int main(int argc, char* argv[]) {
  if (argc != 2) {
    perror("Must pass only the correct shared memory name\n");
    exit(1);
  }

  char shmName[] = argv[1];

  int fd = shm_open(shmName, O_RDONLY, 0666);
  if (fd == -1) {
    perror("Failed to open fd\n");
    exit(1);
  }
  //Only creator has to ftruncate(fd, BLOCK_SIZE);
  float* shm_ptr = mmap(0, BLOCK_SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("Failed to create the shm_ptr\n");
    exit(1);
  }

  struct sigaction sa;
  sa.sa_handler = SSR_SIGUSR1;
  sigaction(SIGUSR1, &sa, NULL);

  while (1) {
    if (isTrue) {
      for (int i = 0; i < 25; i++) {
        printf("SHM[%i] = %f\n", i, shm_ptr[i]);
      }
      isTrue = false;
    }
    pause();
  }
}
