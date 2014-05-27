#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <playerFIFO> <displayFIFO>\n"
                , argv[0]);
        return 1;
    }
    int playfd = open(argv[1], O_RDWR);
    int dispfd = open(argv[2], O_WRONLY);
    char buf[256] = "\0";
    read(playfd, buf, 256);
    if (!strncmp(buf, "first", 5)){
        strncpy(buf, "got first\0", 10);
        write(dispfd, buf, 256);
        sleep(1);
        strncpy(buf, "second\0", 7);
        write(playfd, buf, 256);
        sleep(1);
        read(playfd, buf, 256);
        close(playfd);
        unlink(argv[1]);
    }
    else {
        strncpy(buf, "got second\0", 11);
        write(dispfd, buf, 256);
        strncpy(buf, "confirm\0", 8);
        write(playfd, buf, 256);
    }

    return 0;
}
