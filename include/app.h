#pragma once

#include "command_args.h"

/**
 * @brief Opaque type of an application.
 */
typedef struct app_t app_t;

/**
 * @brief Initializes the application.
 *
 * @param command_args is the command line arguments from the command line
 * @return a pointer to the newly created application
 */
app_t *initialize_app(command_args_t command_args);

/**
 * @brief Runs the application.
 *
 * @param app is the application
 * @param command_args is  the command line arguments from the command line
 */
void run_app(app_t *app, command_args_t command_args);

/**
 * @brief Destroys the application.
 *
 * @param app is the application
 */
void destroy_app(app_t *app);
