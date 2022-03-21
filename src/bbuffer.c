#include "../include/bbuffer.h"
#include "../include/logger.h"
#include "../include/sem.h"

/**
 * @brief The bounded buffer acts as a FIFO queue that holds the client socket
 * file descriptors.
 */
typedef struct BNDBUF {
  unsigned int size;
  unsigned int head;
  unsigned int tail;
  int *buffer;
  SEM *sem_empty;
  SEM *sem_full;
} BNDBUF;

BNDBUF *bb_init(unsigned int size) {
  char message[LOG_MESSAGE_SIZE];
  snprintf(message, sizeof(message),
           "Initializing bounded buffer with %d buffer slots.", size);
  log_info(message);

  BNDBUF *bb = malloc(sizeof(BNDBUF));

  if (!bb) {
    log_error("An error occurred while initializing the bounded buffer.");
    return NULL;
  }

  // Initialize state of the bounded buffer
  bb->size = size;
  bb->head = 0;
  bb->tail = 0;
  bb->buffer = malloc(bb->size * sizeof(int));

  if (!bb->buffer) {
    log_error("An error occurred while initializing the bounded buffer.");
    return NULL;
  }

  // Initially, the number of empty slots is equal to the size of the buffer
  // and the number of full slots is equal to 0.
  bb->sem_empty = sem_init(bb->size);
  bb->sem_full = sem_init(0);

  return bb;
}

void bb_del(BNDBUF *bb) {
  sem_delete(bb->sem_empty);
  sem_delete(bb->sem_full);

  free(bb->buffer);
  free(bb);

  log_info("Successfully deleted bounded buffer.");
}

int bb_get(BNDBUF *bb) {
  // This is the item to get from the FIFO buffer
  int item;

  P(bb->sem_full);

  // Get item from the bounded buffer
  item = bb->buffer[bb->head];
  bb->head++;

  V(bb->sem_empty);

  char message[LOG_MESSAGE_SIZE];
  snprintf(message, sizeof(message),
           "Consumer removed item %d from the bounded buffer.", item);
  log_info(message);

  return item;
}

void bb_add(BNDBUF *bb, int fd) {
  P(bb->sem_empty);

  // Add item to the bounded buffer
  bb->buffer[bb->tail] = fd;
  bb->tail++;

  V(bb->sem_full);

  char message[LOG_MESSAGE_SIZE];
  snprintf(message, sizeof(message),
           "Producer added file descriptor %d to the bounded buffer.", fd);
  log_info(message);
}
