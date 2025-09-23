#include <stdio.h>

#include "./../include/functions.h"
int main(void) {
  AvailableTicketsInit();
  PurchasedTicketsInit();
  TransactionsInit();

  while (1) {
    printf("TICKET REPORT:\n");
    printf("Purchased tickets: %d\n", *purchased_tickets);
    printf("Available tickets: %d\n", *available_tickets);
    printf("Transactions: %d\n", *transactions);
    if (*available_tickets == 0) {
      break;
    }
    sleep(1);
  }

  printf("SOLD OUT...\n");

  shm_unlink("/available_tickets");
  shm_unlink("/purchased_tickets");
  shm_unlink("/transactions");

  return EXIT_SUCCESS;
}
