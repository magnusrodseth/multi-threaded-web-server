#include "../include/logger.h"
#include <stdlib.h>

void log_info(const char *message)
{
  fprintf(stdout, "> [INFO]: %s\n", message);
}

void log_warning(const char *message)
{
  fprintf(stdout, "> [WARNING]: %s\n", message);
}

void log_error(const char *message)
{
  fprintf(stderr, "> [ERROR]: %s\n", message);
}
