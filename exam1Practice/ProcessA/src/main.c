#include <fcntl.h>
#include <mqueue.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define BLOCK_SIZE (sizeof(float) * 25)
#define MAX_MSG 5
#define MAX_SIZE 30

// Global
bool isFinish = false;

void SSR_SIGINT(int signum) { isFinish = true; }

int main(int argc, char* argv[]) {
  if (argc != 3) {
    perror("Must include two parameters\n");
    exit(EXIT_FAILURE);
  }

  struct sigaction sa;
  sa.sa_handler = SSR_SIGINT;
  sigaction(SIGINT, &sa, NULL);

  fprintf(stderr, "Hello from process A ID: %d\n", getpid());

  const char* queueName = argv[1];
  const char* shmName = argv[2];

  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = MAX_MSG;
  attr.mq_msgsize = MAX_SIZE;

  mqd_t queue = mq_open(queueName, O_CREAT | O_RDWR, 0777, &attr);
  if (queue == -1) {
    perror("Failed creating the queue\n");
    exit(EXIT_FAILURE);
  }

  // File descriptor
  int fd = shm_open(shmName, O_CREAT | O_RDWR, 0777);
  if (fd == -1) {
    perror("Failed to open shm\n");
    exit(EXIT_FAILURE);
  }
  ftruncate(fd, BLOCK_SIZE);
  float* shm = mmap(0, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shm == MAP_FAILED) {
    perror("Error creating the block pointer\n");
    exit(EXIT_FAILURE);
  }

  pid_t processB = fork();

  if (processB == 0) {
    execlp("pathtoexecutable", "nameofexecutable", queueName, shmName,
           (char*)NULL);
    perror("Failed creating process B\n");
    exit(EXIT_FAILURE);
  }

  pid_t processC = fork();

  if (processC == 0) {
    execlp("pathtoexecutable", "nameofexecutable", shmName, (char*)NULL);
    perror("Failed creating process C\n");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    sleep(2);
    if (isFinish) {
      break;
    }
    printf("Type one of the following options EXACTLY as they appear:\n");
    printf("Square numbers\n");
    printf("Divide numbers in half\n");
    printf("Plot numbers\n");
    char answer[MAX_SIZE];//30 esta bien
    if (!(fgets(answer, MAX_SIZE, stdin))) break;
    answer[strcspn(answer, "\n")] =
        '\0';  // strip newline, strcspn return the first index where it
               // encounters the caracter that we want in answer
    if (!(strcmp(answer, "Square numbers"))) {
      const char* msg = "Square";
      mq_send(queue, msg, strlen(msg) + 1, 0);
      continue;
    }

    if (!(strcmp(answer, "Divide numbers in half"))) {
      const char* msg = "half";
      mq_send(queue, msg, strlen(msg) + 1, 0);
      continue;
    }

    if (!(strcmp(answer, "Plot numbers"))) {
      kill(processC, SIGUSR1);
      continue;
    }
  }
  printf("Terminating soon...\n");
  kill(processB, SIGINT);
  kill(processC, SIGINT);
  waitpid(processB, NULL, 0);
  waitpid(processC, NULL, 0);
  munmap(shm, BLOCK_SIZE);
  close(fd);
  shm_unlink(shmName);
  mq_close(queue);
  mq_unlink(queueName);
  return 0;
}
