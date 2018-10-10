#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        puts("Usage: ./server routing_host:rh_port port");
        exit(1);
    }
    while (true) {
        if (fork() == 0)
            execl("./tsd", argv[1], argv[2]);
        else {
            wait(NULL);
            sleep(5); // let another server take over
        }
    }
}
