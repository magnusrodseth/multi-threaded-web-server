#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "bbuffer.h"
#include <stdio.h>

/**
 * @brief Opaque type for a worker.
 */
typedef struct worker_t worker_t;

/**
 * @brief The struct passed to the routine that starts a worker thread.
 *
 * This is a public struct, and should thus be defined in the header file.
 */
typedef struct worker_args_t
{
  worker_t *worker;
} worker_args_t;

/**
 * @brief Initializes the worker.
 *
 * @param bb is the bounded buffer provided by the application.
 * @param directory_to_serve is the directory to serve.
 * @return a pointer to the instance of the worker.
 */
worker_t *initialize_worker(BNDBUF *bb, char *directory_to_serve);

/**
 * @brief The main work loop for a worker.
 *
 * See definition of the struct `worker_t` (src/worker.c) for more information on the responsibilities of a worker.
 *
 * @param worker is the instance of the worker.
 */
void run_worker(worker_t *worker);

/**
 * @brief Destroys the worker.
 *
 * @param worker is the instance of the worker.
 */
void destroy_worker(worker_t *worker);

/**
 * @brief The routine executed when creating a new worker thread.
 *
 * This routine runs the worker, and destroys the worker when it is done running.
 *
 * @param args is the instance of the worker.
 * @return a NULL pointer
 */
void *start_worker(void *args);

/**
 * @brief Get the full path of the file to serve.
 *
 * @param worker is the instance of the worker.
 * @param buffer is the buffer containing bytes from the client file descriptor.
 * @return the full path of the file to serve, or NULL if the path is illegal.
 */
char *get_path_from_request(worker_t *worker, char *buffer);
