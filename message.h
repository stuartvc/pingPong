#ifndef MESSAGE_H_
#define MESSAGE_H_

typedef struct pos {
    float x;
    float y;
} pos;

typedef pos dir;

typedef struct message {
    int playerNum;
    char message[20];
    int myPlace;
    pos ballPos;
    dir ballDir;
} message;

void createMessage(char buf[256], message message);
void parseMessage(char buf[256], message *message);

#endif