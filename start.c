#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main() {
    pid_t pid;
    char playerFIFO[30] = "./playerFIFO";
    char displayFIFO[30] = "./displayFIFO";

    char *playerArg[4];
    char *displayArg[4];

    playerArg[0] = "player";
    playerArg[1] = playerFIFO;
    playerArg[2] = displayFIFO;
    playerArg[3] = NULL;
    
    displayArg[0] = "display";
    displayArg[1] = displayFIFO;
    displayArg[2] = NULL;
    
    mkfifo(playerFIFO, 0666);
    mkfifo(displayFIFO, 0666);

    switch ((pid = fork())) 
    {
        case -1:
            fprintf(stderr, "error creating child\n");
            exit(1);
            break;
        case 0:
            execv(displayArg[0], displayArg);
            fprintf(stderr, "error executing display\n");
            exit(2);
            break;
        default:
            //fprintf(stdout, "parent\n");
            break;
    }

    switch ((pid = fork())) 
    {
        case -1:
            fprintf(stderr, "error creating child\n");
            exit(1);
            break;
        case 0:
            execv(playerArg[0], playerArg);
            fprintf(stderr, "error executing player\n");
            exit(2);
            break;
        default:
            //fprintf(stdout, "parent\n");
            break;
    }

    switch ((pid = fork())) 
    {
        case -1:
            fprintf(stderr, "error creating child\n");
            exit(1);
            break;
        case 0:
            execv(playerArg[0], playerArg);
            fprintf(stderr, "error executing player\n");
            exit(2);
            break;
        default:
            //fprintf(stdout, "parent\n");
            break;
    }
    int fd = open("./playerFIFO", O_WRONLY);

    write(fd, "first", 256);
    close(fd);
    sleep(2);

    return 0;
}
