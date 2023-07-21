#ifndef VSCD_SOCKET_H
#define VSCD_SOCKET_H

#define SOCKET_PATH "/var/run/vscd.socket"

#include <stdio.h>
#include <syslog.h>

void create_socket();
ssize_t recieve_from_service(void* buffer, size_t buffer_size);
int send_to_service();
int send_to_daemon();
ssize_t recieve_from_daemon(void* buffer, size_t buffer_size);

struct VscdSockets {
    int server_fd;
    int client_fd;

    void log(const char* info = "") {
        syslog(LOG_NOTICE, "vscd.server_fd: %d. vscd.client_fd: %d",
               this->server_fd, this->client_fd);
    }
};

extern struct VscdSockets vscd_sockets;

#endif /* VSCD_SOCKET_H */
