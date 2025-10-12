#include "./../include/functions.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int fd = 0;
memory* shm_ptr = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void Setup() {
  pthread_mutex_init(&mutex, NULL);
  fd = shm_open(SHM_NAME, O_RDWR, 0777);
  if (fd == -1) {
    perror("Error creating the file descriptor.");
    exit(EXIT_FAILURE);
  }

  shm_ptr = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("Error mapping the shared memory segment.");
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "Connected to the ticket office!\n");
}
void* BuyTickets(void* arg) {
  while (shm_ptr->available_tickets > 0) {
    int buyer_number = (int)(intptr_t)arg;
    int tickets_to_buy = 1 + rand() % 5;  // 1..5
    pthread_mutex_lock(&mutex);
    printf("Buyer %d requests %d tickets.\n", buyer_number, tickets_to_buy);
    if (shm_ptr->available_tickets >= tickets_to_buy) {
      shm_ptr->available_tickets -= tickets_to_buy;
      shm_ptr->purchases_log[shm_ptr->transactions] = tickets_to_buy;
      shm_ptr->transactions += 1;
      printf("Thread %p purchased %d tickets. Available: %d\n",
             (void*)pthread_self(), tickets_to_buy, shm_ptr->available_tickets);
    } else {
      printf(
          "Thread %p failed to purchase tickets. Not enough available "
          "tickets.\n",
          (void*)pthread_self());
    }
    pthread_mutex_unlock(&mutex);
    usleep(1000000);  // Simulate time taken for purchase
  }
  return NULL;
}
