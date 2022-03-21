#include "../include/server.h"
#include "../include/bbuffer.h"
#include "../include/command_args.h"
#include "../include/constants.h"
#include "../include/worker.h"
#include "../include/logger.h"

typedef struct server_t
{
    int socket_fd;
    struct sockaddr_in6 server_address;
    command_args_t command_args;
    bool is_running;
    BNDBUF *bb;
    thread_pool_t *thread_pool;
} server_t;

server_t *initialize_server(command_args_t command_args, BNDBUF *bb)
{
    log_info("Initializing server.");

    int socket_fd;

    socket_fd = socket(AF_INET6, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        char message[LOG_MESSAGE_SIZE];
        snprintf(message, sizeof(message), "An error occurred when opening the socket with file descriptor %d.", socket_fd);
        log_error(message);
        exit(1);
    }

    int set_socket_opt_flag = 1;
    int set_socket_opt_result = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_socket_opt_flag, sizeof(set_socket_opt_flag));

    if (set_socket_opt_result == -1)
    {
        log_error("An error occurred when setting socket options.");
        exit(1);
    }

    // Erases the data in the server address
    struct sockaddr_in6 server_address;
    bzero((char *)&server_address, sizeof(server_address));

    // Support both IPv4 and IPv6 addresses
    server_address.sin6_family = AF_INET6;
    // Converts the character string src into a network address structure
    inet_pton(INADDR_ANY, "::1", &server_address.sin6_addr);
    // Converts the unsigned short integer "hostshort" from host byte order to
    // network byte order.
    server_address.sin6_port = htons(command_args.port);

    // Assigns the server address to the socket
    int bind_result = bind(socket_fd, (struct sockaddr *)&server_address,
                           sizeof(server_address));

    if (bind_result < 0)
    {
        log_error("An error occurred when binding server address to the server socket.");
        exit(1);
    }

    server_t *server = malloc(sizeof(server_t));
    server->socket_fd = socket_fd;
    server->server_address = server_address;
    server->command_args = command_args;
    server->bb = bb;

    return server;
}

int setup_server_socket(server_t *server)
{
    return listen(server->socket_fd, ONE_KILOBYTE);
}

void destroy_server(server_t *server)
{
    free(server);
    log_info("Successfully terminated server.");
}

int wait_for_connection(int socket_fd)
{
    int client_socket_fd = 0;
    struct sockaddr_in client_address = {0};
    socklen_t client_length = sizeof(client_address);

    // Extracts the first connection request on the queue of pending
    // connections for the listening socket
    client_socket_fd =
        accept(socket_fd, (struct sockaddr *)&client_address, &client_length);

    if (client_socket_fd < 0)
    {
        char message[LOG_MESSAGE_SIZE];
        snprintf(message,
                 sizeof(message),
                 "An error occurred when accepting client with socket file descriptor %d.",
                 client_socket_fd);
        log_error(message);
    }

    char message[LOG_MESSAGE_SIZE];
    snprintf(message,
             sizeof(message),
             "Accepted new connection to client with file descriptor %d.",
             client_socket_fd);
    log_info(message);

    return client_socket_fd;
}

void run_server(server_t *server)
{
    server->is_running = true;

    char message[LOG_MESSAGE_SIZE];
    snprintf(message, sizeof(message), "Server started on http://localhost:%d", server->command_args.port);
    log_info(message);

    while (server->is_running)
    {
        int client_socket_fd = wait_for_connection(server->socket_fd);

        // Server is the producer, and adds item to bounded buffer
        bb_add(server->bb, client_socket_fd);
    }
}
