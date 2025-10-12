#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "./../include/functions.h"

int main() {
  Setup();
  while (shm_ptr->available_tickets > 0) {
    printf("\nTICKET REPORT: \n");
    printf("Available tickets: %d\n", shm_ptr->available_tickets);
    sleep(1);
  }
  EndShm();
  return 0;
}
