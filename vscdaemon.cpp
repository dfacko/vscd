#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>

#include "vscd_daemon.h"
#include "vscd_pid.h"
#include "vscd_service.h"
#include "vscd_workingdirectory.h"

static void daemonize() {
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0) exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0) exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0) exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    // TODO: Implement a working signal handler */
    // ssignal(SIGCHLD, SIG_IGN);
    // signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0) exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0) exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    if (chdir("/") < 0) {
        perror("Failed to change directory to /");
        exit(EXIT_FAILURE);
    }

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }

    /* Open the log file */
    openlog("vscdaemon", LOG_PID, LOG_DAEMON);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "daemon") == 0) {
        if (getuid() != 0) {
            // log for user in running terminal
            printf("Daemon requires root priviledges to run.\n");
            exit(EXIT_FAILURE);
        }

        syslog(LOG_NOTICE, "Starting daemon.");

        daemonize();

        if (write_pid() != 0) {
            exit(EXIT_FAILURE);
        };

        register_terminate_signals();

        create_vsc_directories("/var/vsc");

        syslog(LOG_NOTICE, "First daemon started.");

        vscd_daemon();

    } else if (strcmp(argv[1], "service") == 0) {
        // Handle the "service" case
        printf("Starting the service...\n");

        openlog("vscdaemon-service", LOG_PID, LOG_DAEMON);

        vscd_service();

    } else {
        printf("Invalid argument: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /*syslog(LOG_NOTICE, "First daemon terminated.");
    closelog();

    cleanup();
    return EXIT_SUCCESS;*/
}
