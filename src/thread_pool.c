#include "../include/thread_pool.h"
#include "../include/logger.h"
#include "../include/worker.h"

thread_pool_t *initialize_thread_pool(int worker_threads_count) {
  char message[LOG_MESSAGE_SIZE];
  snprintf(message, sizeof(message),
           "Initializing thread pool with %d worker threads.",
           worker_threads_count);
  log_info(message);

  thread_pool_t *thread_pool = malloc(sizeof(thread_pool_t));
  thread_pool->size = worker_threads_count;
  thread_pool->pool = malloc(thread_pool->size * sizeof(pthread_t));
  thread_pool->count = 0;

  for (int i = 0; i < thread_pool->size; i++) {
    thread_pool->pool[i] = 0;
  }

  return thread_pool;
}

void add_thread(thread_pool_t *thread_pool, void *(*thread)(), void *args) {
  if (thread_pool->count >= thread_pool->size) {
    log_warning("Thread pool is full.");
    return;
  }

  pthread_create(&thread_pool->pool[thread_pool->count], NULL, thread, args);

  thread_pool->count++;
}

void destroy_thread_pool(thread_pool_t *thread_pool) {
  // Join worker threads at the end of the application's runtime
  for (int i = 0; i < thread_pool->size; i++) {
    if (thread_pool->pool[i] != 0) {
      pthread_join(thread_pool->pool[i], NULL);
      free(thread_pool->pool[i]);
    }
  }

  log_info("Successfully terminated thread pool.");

  free(thread_pool->pool);
  free(thread_pool);
}
