#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

void sendMessage(int fd, char message[250], int playerNum);
void readMessage(int fd, char message[256]);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <playerFIFO> <displayFIFO>\n"
                , argv[0]);
        return 1;
    }
    int playerNum = -1;
    int playfd = open(argv[1], O_RDWR);
    int dispfd = open(argv[2], O_WRONLY);
    char buf[256] = "\0";
    read(playfd, buf, 256);
    if (!strncmp(buf, "first", 5)){
        playerNum = 1;
        sendMessage(dispfd, "got first", playerNum);
        sleep(1);
        sendMessage(playfd, "second", playerNum);
        sleep(1);
        readMessage(playfd, buf);
        int i = 0;
        while (i++ < 10) {
            sprintf(buf, "player %i sending %i\0", playerNum, i);
            sendMessage(dispfd, buf, playerNum);
            sprintf(buf, "player %i sent %i\0", playerNum, i);
            sendMessage(playfd, buf, playerNum);
            sleep(1);
        }
        sleep(1);
        close(playfd);
        unlink(argv[1]);
    }
    else {
        playerNum = 2;
        sendMessage(dispfd, "got second\0", playerNum);
        sendMessage(playfd, "confirm\0", playerNum);
        sleep(1);
        int i = 0;
        while (i++ < 10) {
            readMessage(playfd, buf);
            sendMessage(dispfd, buf+2, playerNum);
        }
        close(playfd);
    }

    return 0;
}

void sendMessage(int fd, char message[250], int playerNum){
    char buf[256] = "\0";
    sprintf(buf, "%i-%s\0", playerNum, message);
    write(fd, buf, 256);
}

void readMessage(int fd, char message[256]) {
    char buf[256] = "\0";
    int n = read(fd, buf, 256);
    strncpy(message, buf, n);
}

