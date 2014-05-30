#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void createMessage(char buf[256], message message) {
    sprintf(buf, "%i-%i-%f-%f-%f-%f-%s%c",
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
    message->playerNum = buf[i] -'0';
    while (buf[i++] != '-');
    message->myPlace = buf[i] -'0';
    while (buf[i++] != '-');
    message->ballPos.x = atof(buf+i);
    while (buf[i++] != '-');
    message->ballPos.y = atof(buf+i);
    while (buf[i++] != '-');
    message->ballDir.x = atof(buf+i);
    while (buf[i++] != '-');
    message->ballDir.y = atof(buf+i);
    while (buf[i++] != '-');
    strncpy(message->message, buf+i, 20);
}
