#include "../include/sem.h"

/**
 * @brief An implementation of a semaphore using mutex and conditional variables.
 */
typedef struct SEM
{
  int counter;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
} SEM;

SEM *sem_init(int initial_value)
{
  log_info("Initializing semaphore.");

  // Initialize the semaphore
  SEM *sem = malloc(sizeof(SEM));
  pthread_mutex_init(&sem->mutex, NULL);
  pthread_cond_init(&sem->condition, NULL);
  sem->counter = initial_value;

  return sem;
}

int sem_delete(SEM *sem)
{
  log_info("Deleting semaphore.");

  int status = pthread_mutex_destroy(&sem->mutex);

  free(sem);

  if (status != 0)
  {
    return -1;
  }

  return status;
}

void P(SEM *sem)
{
  pthread_mutex_lock(&sem->mutex);

  while (sem->counter == 0)
  {
    pthread_cond_wait(&sem->condition, &sem->mutex);
  }

  sem->counter--;

  pthread_mutex_unlock(&sem->mutex);
}

void V(SEM *sem)
{
  pthread_mutex_lock(&sem->mutex);

  sem->counter++;

  pthread_mutex_unlock(&sem->mutex);

  pthread_cond_signal(&sem->condition);
}
