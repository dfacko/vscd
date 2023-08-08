#ifndef VSCD_SOCKET_H
#define VSCD_SOCKET_H

#define SOCKET_PATH "/var/run/vscd.socket"
#define BINARY_INSTALL_PATH "/usr/local/sbin/vscdaemon"

#include <stdio.h>
#include <syslog.h>

int accept_service_connection(int* client_fd, int* server_fd);

void create_socket(int* server_fd);
ssize_t recieve_from_service(void* buffer, size_t buffer_size, int* client_fd);
int send_to_service(void* buffer, size_t buffer_size, int* client_fd);
int send_to_daemon(void* buffer, size_t buffer_size, int* client_fd);
ssize_t recieve_from_daemon(void* buffer, size_t buffer_size, int* client_fd);

#endif /* VSCD_SOCKET_H */
