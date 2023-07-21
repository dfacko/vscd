#include "vscd_workingdirectory.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#define MAX_PATH 4096

void create_vsc_directories(const char *path) {
    char *current_dir = (char *)malloc(MAX_PATH);

    // Create /var/vsc if it does not exist

    if (chdir(path) < 0) {
        syslog(LOG_NOTICE, "Failed to change directory to %s ", path);

        syslog(LOG_NOTICE, "Trying to create working directory %s", path);

        if (mkdir(path, 0700) < 0) {
            syslog(LOG_NOTICE, "Failed to create vsc directory %s", path);
        } else {
            syslog(LOG_NOTICE, "Created vsc directory %s", path);
        }
    }

    free(current_dir);
    current_dir = NULL;
}
