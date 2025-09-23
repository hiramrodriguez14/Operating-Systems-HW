#include "./../include/functions.h"

#include <stdio.h>
int* available_tickets;
int* purchased_tickets;
int* transactions;

void AttachMemory() {
  int fd1 = shm_open(SHM1_NAME, O_RDWR, 0666);
  ftruncate(fd1, BLOCK_SIZE);
  available_tickets =
      mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

  if (available_tickets == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  int fd2 = shm_open(SHM2_NAME, O_RDWR, 0666);
  ftruncate(fd2, BLOCK_SIZE);
  purchased_tickets =
      mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

  if (purchased_tickets == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  int fd3 = shm_open(SHM3_NAME, O_RDWR, 0666);
  ftruncate(fd3, BLOCK_SIZE);
  transactions =
      mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd3, 0);

  if (transactions == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }
}

void BuyTickets(int tickets) {
  if (tickets <= 0) {
    printf("Try again, invalid number of tickets\n");
    return;
  }
  if (tickets > *available_tickets) {
    tickets = *available_tickets;
  }
  if (tickets > 5) {
    tickets = 5;
  }
  *available_tickets = *available_tickets - tickets;
  *transactions = *transactions + 1;
  *purchased_tickets += tickets;
  printf("Company purchased %d tickets. Available: %d\n", tickets,
         *available_tickets);
  return;
}
