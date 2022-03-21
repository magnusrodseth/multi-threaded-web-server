#pragma once

#include <stdio.h>

#define LOG_MESSAGE_SIZE 256

/**
 * @brief Logs an information message.
 *
 * @param message is the message to log.
 */
void log_info(const char *message);

/**
 * @brief Logs a warning message.
 *
 * @param message is the message to log.
 */
void log_warning(const char *message);

/**
 * @brief Logs an error message.
 *
 * @param message is the message to log.
 */
void log_error(const char *message);
