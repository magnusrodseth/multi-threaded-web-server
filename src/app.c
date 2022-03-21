#include "../include/app.h"
#include "../include/logger.h"
#include "../include/server.h"
#include "../include/thread_pool.h"
#include "../include/worker.h"

/**
 * @brief The application is the middle man between the server and the thread
 * pool of worker threads.
 */
typedef struct app_t
{
  server_t *server;
  thread_pool_t *thread_pool;
  BNDBUF *bb;
} app_t;

app_t *initialize_app(command_args_t command_args)
{
  log_info("Initializing application.");

  app_t *app = malloc(sizeof(app_t));

  // Initialize thread pool with worker threads
  thread_pool_t *thread_pool =
      initialize_thread_pool(command_args.worker_threads_count);
  app->thread_pool = thread_pool;
  app->thread_pool->size = command_args.worker_threads_count;

  // Initialize bounded buffer
  app->bb = bb_init(command_args.buffer_slots);

  return app;
}

void run_app(app_t *app, command_args_t command_args)
{

  server_t *server = initialize_server(command_args, app->bb);
  app->server = server;

  int listen_result = setup_server_socket(server);

  if (listen_result != 0)
  {
    log_error(
        "An error occurred when setting up socket that will accept incoming "
        "connection requests.");
  }

  // Create worker threads in pool, given that the server socket is successfully
  // set up
  for (int i = 0; i < app->thread_pool->size; i++)
  {
    char message[LOG_MESSAGE_SIZE];
    snprintf(message, sizeof(message), "Initializing worker %d.", i + 1);
    log_info(message);

    worker_t *worker =
        initialize_worker(app->bb, command_args.directory_to_serve);

    // Add worker thread to thread pool
    worker_args_t *worker_args = malloc(sizeof(worker_args_t));
    worker_args->worker = worker;
    add_thread(app->thread_pool, start_worker, worker_args);
  }

  run_server(server);
}

void destroy_app(app_t *app)
{
  bb_del(app->bb);
  destroy_thread_pool(app->thread_pool);
  destroy_server(app->server);
  free(app);

  log_info("Successfully terminated application.");
}
