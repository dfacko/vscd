
#include "vscd_daemon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "vscd_socket.h"

void vscd_daemon() {
    int client_fd = 0;
    int server_fd = 0;

    create_socket(&server_fd);

    while (1) {
        if (accept_service_connection(&client_fd, &server_fd) == 0) {
            syslog(LOG_NOTICE, "client_fd is : %d", client_fd);

            unsigned char buffer[1024] = {0};
            ssize_t num_bytes = recieve_from_service(buffer, 1024, &client_fd);

            ((char*)buffer)[num_bytes] = '\0';

            syslog(LOG_NOTICE, "recived from service :%s \n", (char*)buffer);

            memset(buffer, 0, 1024);
            memcpy((char*)buffer, "Hello from root daemon", 1024);

            send_to_service(buffer, strlen((char*)buffer), &client_fd);
        } else {
            syslog(LOG_ERR, "Connection refused, invalid binary");
        }
    }
}
