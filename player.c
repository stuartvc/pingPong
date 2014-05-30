#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "message.h"

void sendMessage(int fd, char message[250], int playerNum);
void sendMessage1(int fd, message message);
void readMessage(int fd, char message[256]);
void readMessage1(int fd, message *message);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <playerFIFO> <displayFIFO>\n"
                , argv[0]);
        return 1;
    }
    message message;
    message.myPlace = 3;
    message.ballPos.x = 152.23;
    message.ballPos.y = 0.0121;
    message.ballDir.x = 0;
    message.ballDir.y = 0;
    int playerNum = -1;
    int playfd = open(argv[1], O_RDWR);
    int dispfd = open(argv[2], O_WRONLY);
    char buf[256] = "\0";
    read(playfd, buf, 256);
    if (!strncmp(buf, "first", 5)){
        message.playerNum = 1;
        playerNum = 1;
        sprintf(message.message, "got first");
        sendMessage1(dispfd, message);
        sleep(1);
        sendMessage(playfd, "second", playerNum);
        sleep(1);
        readMessage(playfd, buf);
        int i = 0;
        while (i++ < 10) {
            sprintf(buf, "player %i sending %i", playerNum, i);
            sprintf(message.message, "player %i sending %i", message.playerNum, i);
            sendMessage1(dispfd, message);
            sprintf(buf, "player %i sent %i", playerNum, i);
            sprintf(message.message, "player %i sent %i", message.playerNum, i);
            sendMessage1(playfd, message);
            sleep(1);
        }
        sleep(1);
        close(playfd);
        unlink(argv[1]);
    }
    else {
        message.playerNum = 2;
        playerNum = 2;
        sprintf(message.message, "got second");
        sendMessage1(dispfd, message);
        sendMessage(playfd, "confirm\0", playerNum);
        sleep(2);
        int i = 0;
        while (i++ < 10) {
            readMessage1(playfd, &message);
            message.playerNum = 2;
            sendMessage1(dispfd, message);
        }
        close(playfd);
    }

    return 0;
}

void sendMessage(int fd, char message[250], int playerNum){
    char buf[256] = "\0";
    sprintf(buf, "%i-%s", playerNum, message);
    write(fd, buf, 256);
}

void readMessage(int fd, char message[256]) {
    char buf[256] = "\0";
    int n = read(fd, buf, 256);
    strncpy(message, buf, n);
}

void sendMessage1(int fd, message message) {
    char buf[256] = "\0";
    createMessage(buf, message);
    write(fd, buf, 256);
}

void readMessage1(int fd, message *message) {
    char buf[256] = "\0";
    int n = read(fd, buf, 256);
    parseMessage(buf, message);
}
