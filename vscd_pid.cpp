#include "vscd_pid.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "vscd_socket.h"

#define PID_PATH "/var/run/vscd.pid"

int write_pid() {
    int pidFile = open(PID_PATH, O_RDWR | O_CREAT, 0644);
    if (pidFile == -1) {
        syslog(LOG_ERR, "Error opening lock file");
        return 1;
    }

    int lockResult = flock(pidFile, LOCK_EX | LOCK_NB);
    if (lockResult == -1) {
        // Another instance of the daemon is already running
        syslog(LOG_ERR, "Another instance of the daemon is already running.\n");
        return 1;
    }

    // Write the PID to the PID file
    pid_t pid = getpid();
    char pidString[16];
    snprintf(pidString, sizeof(pidString), "%d\n", pid);
    if (write(pidFile, pidString, strlen(pidString)) == -1) {
        syslog(LOG_ERR, "Error writing PID file");
        return 1;
    }

    syslog(LOG_NOTICE, "PID file created");
    return 0;
}

void cleanup(int signal) {
    syslog(LOG_NOTICE, "Recieved signal: %d. Cleaning up and shutting down.",
           signal);
    int pidFile = open(PID_PATH, O_RDWR | O_CREAT, 0644);
    if (pidFile == -1) {
        syslog(LOG_ERR, "Error opening PID file");
        exit(EXIT_FAILURE);
    }

    int unlockResult = flock(pidFile, LOCK_UN);
    if (unlockResult == -1) {
        syslog(LOG_ERR, "Error releasing lock");
        exit(EXIT_FAILURE);
    }

    if (remove(PID_PATH) != 0) {
        syslog(LOG_ERR, "Error removing PID file");
        exit(EXIT_FAILURE);
    }

    // Cleanup and close the server socket
    // close(vscd_sockets.server_fd);
    // close(vscd_sockets.client_fd);
    unlink(SOCKET_PATH);

    close(pidFile);
    closelog();
}

void handle_signal(int signal) {
    syslog(LOG_NOTICE, "recieved signal to terminate\n");
    cleanup(signal);
    exit(EXIT_SUCCESS);
}

void register_terminate_signals() {
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    // signal(SIGKILL, handle_signal);
    syslog(LOG_NOTICE, "Registered terminate signals");
}