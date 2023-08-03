
#include "vscd_service.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vscd_socket.h"

void vscd_service() {
    int client_fd = 0;

    void* buffer = malloc(1024);

    memcpy((char*)buffer, "Hello from service", 1024);

    send_to_daemon(buffer, strlen((char*)buffer), &client_fd);

    memset(buffer, 0, 1024);

    ssize_t num_bytes = recieve_from_daemon(buffer, 1024, &client_fd);

    char* char_buffer = (char*)buffer;

    char_buffer[num_bytes] = '\0';

    printf("recived from daemon :%s \n", char_buffer);

    free(buffer);
}