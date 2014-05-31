#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void createMessage(char buf[256], message message) {
    sprintf(buf, "%i_%i_%f_%f_%f_%f_%s%c",
            message.playerNum,
            message.myPlace,
            message.ballPos.x,
            message.ballPos.y,
            message.ballDir.x,
            message.ballDir.y,
            message.message,
            '\0');
}

void parseMessage(char buf[256], message *message) {
    int i = 0;
    message->playerNum = atoi(buf+i);
    while (buf[i++] != '_');
    message->myPlace = atoi(buf+i);
    while (buf[i++] != '_');
    message->ballPos.x = atof(buf+i);
    while (buf[i++] != '_');
    message->ballPos.y = atof(buf+i);
    while (buf[i++] != '_');
    message->ballDir.x = atof(buf+i);
    while (buf[i++] != '_');
    message->ballDir.y = atof(buf+i);
    while (buf[i++] != '_');
    strncpy(message->message, buf+i, 20);
}

void sendMessage(int fd, message message) {
    char buf[256] = "\0";
    createMessage(buf, message);
    write(fd, buf, 256);
}

void readMessage(int fd, message *message) {
    char buf[256] = "\0";
    int n = read(fd, buf, 256);
    parseMessage(buf, message);
}

void initMessage(message *message, int playerNum) {
    message->playerNum = playerNum;
    message->myPlace = 0;
    message->ballPos.x = 0;
    message->ballPos.y = 0;
    message->ballDir.x = 0;
    message->ballDir.y = 0;
    memset(message->message, '\0', 20);
    strncpy(message->message, "cont", 4);
}
