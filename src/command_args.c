#include "../include/command_args.h"
#include "../include/logger.h"

command_args_t parse_command_args(int argc, char **argv)
{
  // Parse number of arguments
  if ((argc - 1) != NUMBER_OF_ARGUMENTS)
  {
    log_error("Incorrect number of input arguments.\n\n"
              "Syntax: \n"
              "$ mtwwwd serve port worker_threads_count buffer_slots\n\n"
              "Run 'make help' for more information.\n");
    exit(1);
  }

  // Parse command line arguments
  command_args_t command_args = {0};
  strncpy(command_args.directory_to_serve, argv[1], strlen(argv[1]));
  command_args.port = atoi(argv[2]);
  command_args.worker_threads_count = atoi(argv[3]);
  command_args.buffer_slots = atoi(argv[4]);

  return command_args;
}
