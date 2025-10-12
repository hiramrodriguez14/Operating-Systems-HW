#include <pthread.h>
#include <stdint.h>  // for intptr_t
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

#define NUM_THREADS 3
int count = 0;

int main(void) {
  pthread_t threads[NUM_THREADS];
  int rc;

  Setup();
  srand((unsigned)time(NULL));

  for (int i = 0; i < NUM_THREADS; i++) {
    // pass a small integer ID safely via intptr_t
    // intptr_t tid = i + 1;  // 1..3
    count++;
    rc = pthread_create(&threads[i], NULL, BuyTickets, (void*)(intptr_t)count);
    if (rc != 0) {
      perror("Error creating thread");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("Buyer finished purchasing tickets.\n");
  return 0;
}
