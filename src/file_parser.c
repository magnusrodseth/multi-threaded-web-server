
#include "../include/file_parser.h"

bool is_index_page(const char *string)
{
  if (string != NULL)
  {
    size_t size = strlen(string);
    if (string[size - 1] == '/')
    {
      return true;
    }
  }

  return false;
}

bool is_illegal_path(char *path)
{
  int directories_up = 0;
  int directories_down = 0;

  char *path_copy = malloc(sizeof(char) * strlen(path));
  strncpy(path_copy, path, sizeof(char) * strlen(path));

  char *split = strtok(path_copy, "/");

  while (split != NULL)
  {
    bool navigated_up = strstr(split, "..");

    if (navigated_up)
    {
      directories_up++;

      // We have an illegal path
      if (directories_up - directories_down > 0)
      {
        log_warning("There was an attempt to navigate outside the server's directory.");
        return true;
      }
    }
    else
    {
      directories_down++;
    }

    split = strtok(NULL, "/");
  }

  return false;
}

bool parse_file(char *path, char *buffer)
{
  if (is_index_page(path))
  {
    strcat(path, "index.html");
  }

  // The variable path now holds the absolute path to the resource we want
  // to provide to the client
  FILE *file = fopen(path, "r");
  char *file_buffer = 0;
  size_t file_length = 0;

  if (file == NULL)
  {
    if (strstr(path, "favicon"))
    {
      return true;
    }

    char message[LOG_MESSAGE_SIZE];
    snprintf(message, sizeof(message), "Could not open the file at \"%s\".", path);
    log_error(message);
    return false;
  }

  int seek_result = fseek(file, 0, SEEK_END);

  if (seek_result != 0)
  {
    char message[LOG_MESSAGE_SIZE];
    snprintf(message, sizeof(message), "Could not open the file at \"%s\".", path);
    log_error(message);

    return false;
  }

  file_length = ftell(file);

  if (file_length == -1)
  {
    char message[LOG_MESSAGE_SIZE];
    snprintf(message, sizeof(message), "Could not open the file at \"%s\".", path);
    log_error(message);

    return false;
  }

  seek_result = fseek(file, 0, SEEK_SET);

  if (seek_result != 0)
  {
    char message[LOG_MESSAGE_SIZE];
    snprintf(message, sizeof(message), "Could not open the file at \"%s\".", path);
    log_error(message);
    return false;
  }

  file_buffer = malloc(file_length + 1);

  if (file_buffer)
  {
    int i = fread(buffer, 1, file_length, file);
    buffer[i] = '\0';
  }

  fclose(file);

  return true;
}