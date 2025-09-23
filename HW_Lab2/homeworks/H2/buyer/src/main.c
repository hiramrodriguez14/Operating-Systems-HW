#include <stdio.h>
#include <stdlib.h>

#include "./../include/functions.h"
int main(void) {
  AttachMemory();
  while (*available_tickets != 0) {
    fprintf(stderr, "Please enter tickets to purchase: \n");
    int tickets;
    scanf("%d", &tickets);
    BuyTickets(tickets);
    sleep(2);
  }
  return 0;
}
