#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <worker1_pid> <worker2_pid>\n", argv[0]);
    return EXIT_FAILURE;
  }

  pid_t worker1 = (pid_t)atoi(argv[1]);
  pid_t worker2 = (pid_t)atoi(argv[2]);

  FILE* f = fopen("./../test/commands.txt", "r");
  if (!f) {
    perror("Failed to open commands.txt");
    return EXIT_FAILURE;
  }
  char buffer[256];

  char worker[16];
  char signal[16];

  while (fgets(buffer, sizeof(buffer), f)) {
    // Try to read two words: <worker> <signal>
    if (sscanf(buffer, "%15s %15s", worker, signal) == 2) {
      if (strcmp(worker, "worker1") == 0) {
        if (strcmp(signal, "SIGUSR1") == 0) {
          fprintf(stderr, "Sent SIGUSR1 to worker1 (PID: %d)\n", worker1);
          kill(worker1, SIGUSR1);

        } else if (strcmp(signal, "SIGUSR2") == 0) {
          fprintf(stderr, "Sent SIGUSR2 to worker1 (PID: %d)\n", worker1);
          kill(worker1, SIGUSR2);
        }
      } else if (strcmp(worker, "worker2") == 0) {
        if (strcmp(signal, "SIGUSR1") == 0) {
          fprintf(stderr, "Sent SIGUSR1 to worker2 (PID: %d)\n", worker2);
          kill(worker2, SIGUSR1);
        } else if (strcmp(signal, "SIGUSR2") == 0) {
          fprintf(stderr, "Sent SIGUSR2 to worker2 (PID: %d)\n", worker2);
          kill(worker2, SIGUSR2);
        }
      }
    }

    sleep(1);  // wait for 1 second before reading the next line
  }

  fclose(f);
  // Terminate workers
  kill(worker1, SIGTERM);
  kill(worker2, SIGTERM);
  return EXIT_SUCCESS;
}
