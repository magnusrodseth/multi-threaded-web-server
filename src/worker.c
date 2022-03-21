#include "../include/worker.h"
#include "../include/bbuffer.h"
#include "../include/logger.h"
#include "../include/file_parser.h"

/**
 * @brief The worker has several responsibilities:
 *
 * (1) Receive the request on the client socket passed into the bounded buffer.
 * (2) Parse the request from the client.
 * (3) Parse the file that the client is requesting.
 * (3) Send response to the client.
 * (4) Close the connection to the client.
 * (5) Start with the next request.
 */
typedef struct worker_t
{
  char directory_to_serve[DIRECTORY_TO_SERVE_SIZE];
  BNDBUF *bb;
} worker_t;

void *start_worker(void *args)
{
  // Parse input args
  worker_args_t *worker_args = (worker_args_t *)args;
  worker_t *worker = worker_args->worker;

  run_worker(worker);
  destroy_worker(worker);

  free(args);

  pthread_exit(NULL);
}

worker_t *initialize_worker(BNDBUF *bb, char *directory_to_serve)
{

  worker_t *worker = malloc(sizeof(worker_t));
  worker->bb = bb;
  strncpy(worker->directory_to_serve, directory_to_serve,
          strlen(directory_to_serve));

  return worker;
}

void run_worker(worker_t *worker)
{
  // A worker thread should live throughout the lifetime of the application.
  // Hence, we have a while true loop here.
  while (true)
  {
    // Assign worker to a client_socket_fd
    int client_socket_fd = bb_get(worker->bb);

    char buffer[FOUR_KILOBYTES];
    char body[FOUR_HUNDRED_KILOBYTES];
    char message[FOUR_KILOBYTES];

    bzero(buffer, sizeof(buffer));

    int request_length =
        read(client_socket_fd, buffer, sizeof(buffer) - 1);

    if (request_length < 0)
    {
      char message[LOG_MESSAGE_SIZE];
      snprintf(message,
               sizeof(message),
               "An error occurred when reading from client socket file descriptor %d",
               client_socket_fd);
      log_error(message);
    }

    log_info("Worker successfully read request from client.");

    char *path = get_path_from_request(worker, buffer);

    if (path != NULL)
    {

      bool file_parsed_successfully = parse_file(path, body);

      if (file_parsed_successfully)
      {
        // Response to client
        snprintf(message, sizeof(message),
                 "HTTP/0.9 200 OK\n"
                 "Content-Type: text/html\n"
                 "Content-Length: %lu\n\n%s",
                 strlen(body), body);
      }
      else
      {

        bool page_not_found_parsed_successfully = parse_file("static/404.html", body);

        if (!page_not_found_parsed_successfully)
        {
          strncpy(body, "<!DOCTYPE html>"
                        "<html lang=\"en\">"
                        "<head>"
                        "<meta charset = \"UTF-8\">"
                        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                        "<title> 404 Page Not Found</ title>"
                        "</head>"
                        "<body>"
                        "<h1>404 Page Not Found</h1>"
                        "</body>"
                        "</html>",
                  sizeof(body));
        }
        // Response to client
        snprintf(message, sizeof(message),
                 "HTTP/0.9 404 Page not found\n"
                 "Content-Type: text/html\n"
                 "Content-Length: %lu\n\n%s",
                 strlen(body), body);
      }
    }

    request_length = write(client_socket_fd, message, strlen(message));

    if (request_length < 0)
    {
      log_error("An error occurred when writing to socket.");
    }

    log_info("Worker successfully sent response to client.");

    close(client_socket_fd);

    free(path);
  }
}

void destroy_worker(worker_t *worker)
{
  free(worker->directory_to_serve);
  free(worker);
}

char *get_path_from_request(worker_t *worker, char *buffer)
{
  char *request;
  char *location;
  char *resource;

  char *delimiter = " ";

  request = strtok(buffer, "\n");
  location = strtok(request, delimiter);
  resource = strtok(NULL, delimiter);

  if (is_illegal_path(resource))
  {
    char message[LOG_MESSAGE_SIZE];
    snprintf(message,
             sizeof(message),
             "Illegal path \"%s\".",
             resource);
    log_warning(message);

    return (char *)NULL;
  }

  // We add 20 to the size for some leeway.
  // See the is_index_page for context.
  int size = strlen(worker->directory_to_serve) + strlen(resource) + 20;
  char *path = calloc(1, size);

  strncpy(path, worker->directory_to_serve, strlen(worker->directory_to_serve));
  strcat(path, resource);

  return path;
}