#ifndef TICKET_OFFICE_INCLUDE_FUNCTIONS_H_
#define TICKET_OFFICE_INCLUDE_FUNCTIONS_H_
#define SHM_NAME "/ticket_office"
#define BLOCK_SIZE (size_t)sizeof(memory)
extern int fd;
typedef struct {
  int available_tickets;
  int purchases_log[100];
  int transactions;
} memory;

extern memory* shm_ptr;
void Setup();
void EndShm();
#endif  // TICKET_OFFICE_INCLUDE_FUNCTIONS_H_
