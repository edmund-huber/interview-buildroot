#include <fcntl.h>
#include <linux/watchdog.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

int main(void) {
    openlog("watchdog_petter", LOG_PERROR | LOG_PID, LOG_USER);
    syslog(LOG_INFO, "start!");

    int fd = open("/dev/watchdog", O_WRONLY);
    if (fd == -1) {
        return 1;
    }

    while (1) {
        ioctl(fd, WDIOC_KEEPALIVE, 0);
        sleep(2);
    }

    return 0;
}
