
#include "vscd_service.h"

#include <stdio.h>
#include <stdlib.h>

#include "vscd_socket.h"

void vscd_service() {
    vscd_sockets.log("service ");

    void* buffer = malloc(1024);
    send_to_daemon();

    vscd_sockets.log("service ");

    ssize_t num_bytes = recieve_from_daemon(buffer, 1024);

    vscd_sockets.log("service ");

    char* char_buffer = (char*)buffer;

    char_buffer[num_bytes] = '\0';

    printf("recived from daemon :%s \n", char_buffer);

    free(buffer);
}