#ifndef BUYER_INCLUDE_FUNCTIONS_H_
#define BUYER_INCLUDE_FUNCTIONS_H_
#define SHM_NAME "/ticket_office"
#include <pthread.h>
#include <stddef.h>
#include <time.h>
#define BLOCK_SIZE (size_t)sizeof(memory)
extern int fd;
typedef struct {
  int available_tickets;
  int purchases_log[100];
  int transactions;
} memory;
extern pthread_mutex_t mutex;
extern memory* shm_ptr;
extern int count;
void Setup();
void* BuyTickets(void* arg);
#endif  // BUYER_INCLUDE_FUNCTIONS_H_
