
#include "vscd_daemon.h"

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "vscd_socket.h"

void vscd_daemon() {
    vscd_sockets.log();

    while (1) {
        void* buffer = malloc(1024);
        ssize_t num_bytes = recieve_from_service(buffer, 1024);

        vscd_sockets.log();

        send_to_service();

        vscd_sockets.log();

        // char* char_buffer = (char*)buffer;

        ((char*)buffer)[num_bytes] = '\0';

        syslog(LOG_NOTICE, "recived from service :%s \n", (char*)buffer);

        free(buffer);
    }
}
