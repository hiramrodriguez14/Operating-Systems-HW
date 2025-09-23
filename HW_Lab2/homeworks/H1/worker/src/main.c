#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char filename[64];

void SSR_SIGUSR_handler(int signum) {
  FILE* f = fopen(filename, "a");
  if (!f) return;

  if (signum == SIGUSR1) {
    fprintf(f, "SIGUSR1 received\n");
  } else if (signum == SIGUSR2) {
    fprintf(f, "SIGUSR2 received\n");
  }

  fclose(f);
}

int main() {
  pid_t worker = getpid();
  fprintf(stderr, "Worker PID: %d\n", worker);
  // Create file name
  snprintf(filename, sizeof(filename), "worker_log_%d.txt", worker);

  struct sigaction sa;
  sa.sa_handler = SSR_SIGUSR_handler;
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);

  // Wait for signals indefinitely
  while (1) {
    pause();
  }

  return EXIT_SUCCESS;
}
