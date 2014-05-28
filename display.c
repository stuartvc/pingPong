#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <displayFIFO>\n"
                , argv[0]);
        return 1;
    }
    int dispfd = open(argv[1], O_RDONLY);
    char buf[256] = "\0";
    initscr();

    read(dispfd, buf, 256);
    printw("got message: %s\n", buf);
    refresh();
    read(dispfd, buf, 256);
    printw("got message: %s\n", buf);
    refresh();
    sleep(1);
    endwin();
    close(dispfd);
    unlink(argv[1]);

    return 0;
}
