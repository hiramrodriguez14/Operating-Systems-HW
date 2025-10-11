#include <stdio.h>
#include <stdlib.h>

#include "./../include/include.h"

typedef struct {
  int balance;
  char name[20];
} Account;

Account account_a;
Account account_b;

void InitAccount(int balance, char* name, Account* account) {
  account->balance = balance;
  strcpy(account->name, name);

  strcpy(account_a.name, "Account A");
}

void* transfer1(void* arg) {
  pthread_mutex_lock(&mutex);
  account_b.balance += 150;
  account_a.balance -= 150;
  pthread_mutex_lock(&mutex);
}

void* transfer2(void* arg) {
  pthread_mutex_lock(&mutex);
  account_a.balance += 300;
  account_b.balance -= 300;
  pthread_mutex_lock(&mutex);
}
int main(void) {
  pthread_t thread1;
  pthread_t thread2;
  
  InitAccount(300, "Account A", &account_a);
  InitAccount(500, "Account B", &account_b);

  int transferA = pthread_create(&thread1, NULL, transfer1, NULL);
  int transferB = pthread_create(&thread2, NULL, transfer2, NULL);
  if (transferA == -1 || transferB == -1) {
    perror("Failed to created thread 2");
    exit(1);
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("balance: %d   name: %s \n", account_a.balance, account_a.name);
  printf("balance: %d   name: %s \n", account_b.balance, account_b.name);

  return 0;
}
