#pragma once

#include <pthread.h>
#include <stdlib.h>

/**
 * @brief The thread pool manages the worker threads.
 *
 * This is a public struct, and should thus be defined in the header file.
 */
typedef struct thread_pool_t
{
    unsigned int size;
    int count;
    pthread_t *pool;
} thread_pool_t;

/**
 * @brief Initializes a thread pool of worker threads.
 *
 * @param worker_threads_count is the size of the thread pool, and correspondingly the number of worker threads to initialize.
 * @return a pointer to the newly created thread pool.
 */
thread_pool_t *initialize_thread_pool(int worker_threads_count);

/**
 * @brief Adds a worker thread to the thread pool.
 *
 * @param thread_pool is a pointer to the thread pool.
 * @param thread is the routine that runs when creating the worker thread.
 * @param args is the parameters to the routine that runs when creating the worker thread.
 */
void add_thread(thread_pool_t *thread_pool, void *(*thread)(), void *args);

/**
 * @brief Destroys the thread pool and joins worker threads that are currently running.
 *
 * @param thread_pool is a pointer to the thread pool to destroy.
 */
void destroy_thread_pool(thread_pool_t *thread_pool);
