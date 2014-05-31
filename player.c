#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "message.h"

int play(int playfd, int dispfd, int playerNum);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <playerFIFO> <displayFIFO>\n"
                , argv[0]);
        return 1;
    }
    message message;
    message.myPlace = 1;
    message.ballPos.x = 40;
    message.ballPos.y = 15;
    message.ballDir.x = 1;
    message.ballDir.y = 1;
    int playerNum = -1;
    int playfd = open(argv[1], O_RDWR);
    int dispfd = open(argv[2], O_WRONLY);
    char buf[256] = "\0";
    read(playfd, buf, 256);
    if (!strncmp(buf, "first", 5)){
        message.playerNum = 1;
        playerNum = 1;
        sprintf(message.message, "got first");
        sendMessage(dispfd, message);
        usleep(100000);
        sendMessage(playfd, message);
        usleep(100000);
        readMessage(playfd, &message);
        usleep(100000);
        /*int i = 0;
        while (i++ < 10) {
            message.myPlace++;
            message.playerNum = 1;
            sprintf(message.message, "player %i sending %i", message.playerNum, i);
            sendMessage(dispfd, message);
            sprintf(message.message, "player %i sent %i", message.playerNum, i);
            sendMessage(playfd, message);
            sleep(1);
        }
        sleep(1);*/
        message.playerNum = 1;
        sprintf(message.message, "end");
        sendMessage(playfd, message);
        play(playfd, dispfd, 1);
        close(playfd);
        unlink(argv[1]);
    }
    else {
        message.playerNum = 2;
        playerNum = 2;
        sprintf(message.message, "got second");
        sendMessage(dispfd, message);
        sendMessage(playfd, message);
        usleep(200000);
        /*int i = 0;
        while (i++ < 10) {
            readMessage(playfd, &message);
            message.playerNum = 2;
            sendMessage(dispfd, message);
        }*/
        play(playfd, dispfd, 2);
        close(playfd);
    }

    return 0;
}

int play(int playfd, int dispfd, int playerNum) {
    message message0;
    initMessage(&message0, playerNum);
    message message1;
    while (true) {
        do {
            //read message from player
            readMessage(playfd, &message1);
            //figure out paddle place
            message0.myPlace=message1.ballPos.y;
            if (message0.myPlace < 2)
                message0.myPlace = 2;
            if (message0.myPlace > 21)
                message0.myPlace = 21;
            message0.ballPos.x=message1.ballPos.x;
            message0.ballPos.y=message1.ballPos.y;
            message0.ballDir.x=message1.ballDir.x;
            message0.ballDir.y=message1.ballDir.y;
            //send pladdle place to display
            sendMessage(dispfd, message0);
            usleep(100000);
        } while(strncmp(message1.message, "end", 3));
        if (!strncmp(message1.message, "endover", 7))
            return 0;
        do {
            //figure out new ball place
            message0.ballPos.x += message0.ballDir.x;
            message0.ballPos.y += message0.ballDir.y;
            if (message0.ballPos.y < 2)
                message0.ballDir.y *=-1;
            if (message0.ballPos.y > 21)
                message0.ballDir.y *=-1;
            //figure out paddle place
            message0.myPlace = message0.ballPos.y;
            if (message0.myPlace < 2)
                message0.myPlace = 2;
            if (message0.myPlace > 21)
                message0.myPlace = 21;
            //figure out if send to player
            //send to display
            sendMessage(dispfd, message0);
            //send to player
            sendMessage(playfd, message0);
            usleep(100000);
            //figure out if need to send end
        } while(message0.ballPos.x <= 73 && message0.ballPos.x >= 4 );
        if (message0.myPlace + 1 <= message0.ballPos.y || message0.myPlace - 1 >= message0.ballPos.y ) {
            strncpy(message0.message, "endover", 7);
            return -1;
        }
        else {
            message0.ballDir.x *= -1;
            strncpy(message0.message, "end", 3);
            sendMessage(playfd, message0);
            strncpy(message0.message, "cont", 4);
        }
        //check if hit paddle or gone past
        //if hit paddle figure out new speed
        //send "end" to other player
        //else do end of game thing
    }
}
