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
#define MAX_MSG 5
#define MAX_SIZE 30
#define BLOCK_SIZE sizeof(float) * 25
int main(int argc, char* argv) {
  if (argc != 3) {
    perror("Must pass queue name and shm name\n");
    exit(1);
  }

  const char* queueName = argv[1];
  const char *shmName = argv[2];

  struct mq_attr attr;
  attr.flags = 0;
  attr.maxmsg = MAX_MSG;
  attr.msgsize = MAX_SIZE;

  mqd_t queue = mq_open(queueName, O_RDONLY, 0666, &attr);
  if (queue == -1) {
    perror("failed to open queue");
    exit(1);
  }

  int fd = shm_open(shmName, O_RDWR, 0666);
  if (fd == -1) {
    perror("Failed to open fd\n");
    exit(1);
  }
  //Only creator has to ftruncate(fd, BLOCK_SIZE);
  float* shm_ptr =
      mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("Failed to open shm_ptr\n");
    exit(1);
  }

  for (int i = 0; i < 25; i++) {
    shm_ptr[i] = 2.0;
    fprintf(stderr, "SHM[%i] = %f\n", i, shm_ptr[i]);
  }

  while (1) {
    sleep(1);
    char message[30];
    unsigned int priority;

    ssize_t n = mq_receive(queue, message, 30, &priority);

    if (n == -1) {
      if (errno == EAGAIN) {
        // Queue empty
        fprintf(stderr, "Queue empty\n");
        continue;
      } else {
        perror("Failed reading from queue\n");
        exit(1);
      }
    }

    //Strcmp reeturn 0 if equal
    if (!(strcmp(message, "Square"))) {
      for (int i = 0; i < 25; i++) {
        shm_ptr[i] *= shm_ptr[i];
        fprintf(stderr, "SHM[%i] = %f\n", i, shm_ptr[i]);
      }
      continue;
    }

    if ((!strcmp(message, "half"))) {
      for (int i = 0; i < 25; i++) {
        shm_ptr[i] = shm_ptr[i]/2;
        fprintf(stderr, "SHM[%i] = %f\n", i, shm_ptr[i]);
      }
      continue;
    }
  }
}
