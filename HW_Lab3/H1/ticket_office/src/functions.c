#include "./../include/functions.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int fd = 0;
memory* shm_ptr = NULL;

void Setup() {
  shm_unlink(SHM_NAME);
  fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0777);
  if (fd == -1) {
    perror("Error creating the file descriptor.");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fd, sizeof(memory)) == -1) {
    perror("Error setting the size of the shared memory segment.");
    exit(EXIT_FAILURE);
  }
  shm_ptr = mmap(NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("Error mapping the shared memory segment.");
    exit(EXIT_FAILURE);
  }
  shm_ptr->available_tickets = 100;
  shm_ptr->transactions = 0;
  memset(
      shm_ptr->purchases_log, 0,
      sizeof(shm_ptr->purchases_log));  // Set a memory block to a fixed value
  fprintf(stderr, "Ticket office is open!\n");
  printf("Available tickets: %d\n", shm_ptr->available_tickets);
}

void EndShm() {
  // shm_ptr->available_tickets = 0;
  printf("\nTICKET REPORT: \n");
  printf("Available tickets: %d\n", shm_ptr->available_tickets);
  close(fd);
  shm_unlink(SHM_NAME);
  printf("SOLD OUT...");
}
