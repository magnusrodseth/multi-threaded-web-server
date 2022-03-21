#pragma once

#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

#include "logger.h"
#include <stdio.h>

/**
 * Semaphore implementation for the synchronization of POSIX threads.
 *
 * This module implements counting P/V semaphores suitable for the
 * synchronization of POSIX threads. POSIX mutexes and condition variables
 * shall be utilized to implement the semaphore operations.
 **/

/**
 * Opaque type of a semaphore.
 **/
typedef struct SEM SEM;

/**
 * Creates a new semaphore.
 *
 * This function creates a new semaphore. If an error occurs during the
 * initialization, the implementation shall free all resources already
 * allocated so far.
 *
 * Parameters:
 *
 * initial_value - the initial value of the semaphore
 *
 * Returns:
 *
 * a handle for the created semaphore, or NULL if an error occurred.
 **/
SEM *sem_init(int initial_value);

/**
 * Destroys a semaphore and frees all associated resources.
 *
 * Parameters:
 *
 * sem - handle of the semaphore to destroy
 *
 * Returns:
 *
 * 0 on success, negative value on error.
 *
 * In case of an error, not all resources may have been freed, but
 * nevertheless the semaphore handle must not be used any more.
 **/
int sem_delete(SEM *sem);

/**
 * P (wait) operation.
 *
 * Attempts to decrement the semaphore value by 1. If the semaphore value
 * is 0, the operation blocks until a V operation increments the value and
 * the P operation succeeds.
 *
 * Parameters:
 *
 * sem - handle of the semaphore to decrement
 **/
void P(SEM *sem);

/**
 * V (signal) operation.
 *
 * Increments the semaphore value by 1 and notifies P operations that are
 * blocked on the semaphore of the change.
 *
 * Parameters:
 *
 * sem - handle of the semaphore to increment
 **/
void V(SEM *sem);
