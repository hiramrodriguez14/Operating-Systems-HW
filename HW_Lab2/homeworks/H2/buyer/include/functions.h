#ifndef INCLUDE_FUNCTIONS_H_
#define INCLUDE_FUNCTIONS_H_
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM1_NAME "/available_tickets"
#define SHM2_NAME "/purchased_tickets"
#define SHM3_NAME "/transactions"
#define BLOCK_SIZE sizeof(int)

void BuyTickets(int tickets);
void AttachMemory();

extern int* available_tickets;  // declare global pointer
extern int* purchased_tickets;
extern int* transactions;

#endif  // INCLUDE_FUNCTIONS_H_
