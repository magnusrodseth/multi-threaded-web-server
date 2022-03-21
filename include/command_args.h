#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "constants.h"

/**
 * @brief The command arguments holds the command line arguments from the command line.
 *
 * This is a public struct, and should thus be defined in the header file.
 */
typedef struct command_args_t
{
    char directory_to_serve[DIRECTORY_TO_SERVE_SIZE];
    int port;
    int worker_threads_count;
    int buffer_slots;
} command_args_t;

/**
 * @brief Parses the command line arguments into useful data to be used in the application.
 *
 * @param argc is the number of command line arguments.
 * @param argv is the command line arguments.
 * @return an instance of command_args_t.
 */
command_args_t parse_command_args(int argc, char **argv);