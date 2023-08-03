#include "vscd_socket.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>

int accept_service_connection(int* client_fd, int* server_fd) {
    struct sockaddr_un address;

    int addrlen = sizeof(address);

    syslog(LOG_NOTICE, "before accept %d", *client_fd);

    if ((*client_fd = accept(*server_fd, (struct sockaddr*)&address,
                             (socklen_t*)&addrlen)) < 0) {
        syslog(LOG_ERR, "Accept failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}

ssize_t recieve_from_service(void* buffer, size_t buffer_size, int* client_fd) {
    ssize_t num_bytes;

    // Receive message from client
    // MSG_WAITALL
    syslog(LOG_NOTICE, "recv on :%d", *client_fd);

    if ((num_bytes = recv(*client_fd, buffer, buffer_size, 0)) == -1) {
        syslog(LOG_ERR, "Recv failed");
        exit(EXIT_FAILURE);
    }

    // Print the received message
    syslog(LOG_NOTICE, "Received %zd bytes from client\n", num_bytes);

    // Close the client socket
    // close(vscd_sockets.client_fd);

    return num_bytes;
}

int send_to_service(void* buffer, size_t buffer_size, int* client_fd) {
    // Send the reply to the service daemon
    int sent = send(*client_fd, buffer, buffer_size, 0);
    if (sent == -1 || sent != buffer_size) {
        perror("Failed to send reply to service daemon");
        // close(vscd_sockets.client_fd);
    }

    syslog(LOG_NOTICE, "Response sent to the service daemon.\n");
    close(*client_fd);

    return sent;
}

void create_socket(int* server_fd) {
    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Create socket
    if ((*server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        syslog(LOG_ERR, "socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);

    // Bind socket to a file system path
    if (bind(*server_fd, (struct sockaddr*)&address,
             sizeof(struct sockaddr_un)) == -1) {
        syslog(LOG_ERR, "bind failed");
        exit(EXIT_FAILURE);
    }
    /*
        // Set socket to non-blocking mode
        if (fcntl(*server_fd, F_SETFL, O_NONBLOCK) == -1) {
            perror("fcntl failed");
            exit(EXIT_FAILURE);
        }*/

    // Listen for incoming connections
    if (listen(*server_fd, 1) == -1) {
        syslog(LOG_ERR, "listen failed");
        exit(EXIT_FAILURE);
    }

    syslog(LOG_NOTICE, "Root daemon listening on socket: %s\n", SOCKET_PATH);

    return;
}

int send_to_daemon(void* buffer, size_t buffer_size, int* client_fd) {
    struct sockaddr_un server_address;

    // Create socket
    if ((*client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, SOCKET_PATH,
            sizeof(server_address.sun_path) - 1);

    // Connect to server
    if (connect(*client_fd, (struct sockaddr*)&server_address,
                sizeof(server_address)) == -1) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    // Send message to root daemon
    if (send(*client_fd, buffer, buffer_size, 0) == -1) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }

    // Close the socket
    // close(*client_fd);

    return 0;
}

ssize_t recieve_from_daemon(void* buffer, size_t buffer_size, int* client_fd) {
    ssize_t num_bytes = recv(*client_fd, buffer, buffer_size, 0);
    if (num_bytes == -1) {
        syslog(LOG_ERR, "Failed to receive response");
        // no need to close socket to retry?
        // close(vscd_sockets.client_fd);
        return num_bytes;
    }

    printf("Received %zd bytes from root daemon.\n", num_bytes);

    close(*client_fd);

    return num_bytes;
}