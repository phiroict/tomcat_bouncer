#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define EVENT_BUF_LEN     (1024 * (EVENT_SIZE + 16))

void execute_command(const char *command) {
    int ret = system(command);
    if (ret == -1) {
        perror("system");
    } else {
        printf("Command executed with return code: %d\n", ret);
    }
}

int main(int argc, char **argv) {
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path_to_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize inotify
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Add watch to the directory
    wd = inotify_add_watch(fd, argv[1], IN_CREATE);
    if (wd == -1) {
        fprintf(stderr, "Couldn't add watch to %s\n", argv[1]);
        perror("inotify_add_watch");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Watching: %s\n", argv[1]);

    // Loop to monitor directory events
    while (1) {
        length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
            break;
        }

        i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (!(event->mask & IN_ISDIR)) {
                        printf("New file %s created.\n", event->name);
                        // Restart Tomcat service
                        execute_command("systemctl restart tomcat.service");
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    // Clean up
    inotify_rm_watch(fd, wd);
    close(fd);

    return 0;
}
