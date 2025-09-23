#include "./../include/functions.h"

#include <stdio.h>
int* available_tickets;
int* purchased_tickets;
int* transactions;

void AvailableTicketsInit() {
  int fd = shm_open(SHM1_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  available_tickets =
      mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (available_tickets == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  *available_tickets = 20;
}
void PurchasedTicketsInit() {
  int fd = shm_open(SHM2_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  purchased_tickets =
      mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (purchased_tickets == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  *purchased_tickets = 0;
}
void TransactionsInit() {
  int fd = shm_open(SHM3_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(fd, BLOCK_SIZE);
  transactions = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (transactions == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  *transactions = 0;
}
