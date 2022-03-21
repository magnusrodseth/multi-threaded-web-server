#pragma once

#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "bbuffer.h"
#include "command_args.h"
#include "thread_pool.h"
#include <stdlib.h>

/**
 * @brief Opaque type of a server.
 */
typedef struct server_t server_t;

/**
 * @brief Initializes the web server.
 *
 * @param command_args is the command arguments from the command line.
 * @param bb is the bounded buffer that the server will interact with.
 * @return a pointer to the instance of the server.
 */
server_t *initialize_server(command_args_t command_args, BNDBUF *bb);

/**
 * @brief Listens to the server's incoming connections using the server socket.
 *
 * @param server is the instance of the server
 * @return the result of listening to incoming connections.
 */
int setup_server_socket(server_t *server);

/**
 * @brief Waits for a connection by a client.
 *
 * @param socket_fd is the server's socket file descriptor.
 * @return the client file descriptor.
 */
int wait_for_connection(int socket_fd);

/**
 * @brief Destroys the server instance.
 *
 * @param server is the server instance.
 */
void destroy_server(server_t *server);

/**
 * @brief Runs the server, waits for a client connection, and adds the client file descriptor to the bounded buffer.
 *
 * @param server is the instance of the server.
 */
void run_server(server_t *server);
