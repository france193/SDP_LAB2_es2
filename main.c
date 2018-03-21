/**
 * Name:    Francesco
 * Surname: Longo
 * ID:      223428
 * Lab:     2
 * Ex:      1
 *
 * Write a C program that generate a child. The parent opens a file given as argument in the command line,
 * then loops forever,
 * 1) reading each line,
 * 2) printing the line number and the line content
 * 3) rewinding the file.
 * The child process sends a SIGUSR1 signal to the parent at random intervals between 1 and 10 seconds.
 * The first received signal makes the parent skip the print statement. It will restart printing after receiving
 * the next SIGUSR1 signal. This behavior is repeated for all the received SIGUSR1 signals. After 60 seconds,
 * the child must send a SIGUSR2 signal to the parent, and then terminate. Receiving this signal, also the
 * parent will terminate.
 * Hint: the child forks a process that sleeps 60 seconds then send to it a SIGUSR2, which the child catches
 * and “forward” to the parent.
 **/

#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define N 30
#define SEC 60

bool print = true;

void child_manager(int signal);
void parent_manager(int signal);
void parent(char *filename);
void child();
int getRandomNumber(unsigned int seed, int min, int max);

int main(int argc, char **argv) {
    fprintf(stdout, "> Start\n");

    if (argc != 2) {
        fprintf(stdout, "Expected 3 argument: <prog_name> <filename>\n");
        exit(-1);
    }

    // handler for SIGUSR1 & error check
    if(signal(SIGUSR1, parent_manager) == SIG_ERR){
        printf("Signal Handler Error.\n");
        exit(1);
    }

    // handler for SIGUSR2 & error check
    if(signal(SIGUSR2, parent_manager) == SIG_ERR){
        printf("Signal Handler Error.\n");
        exit(1);
    }

    switch(fork()) {
        case 0:
            // child
            child();
            break;

        case -1:
            // error
            fprintf(stdout, "Error, fork error\n");
            exit(-5);

        default:
            // father
            parent(argv[1]);
            break;
    }

    return 0;
}

void parent(char *filename) {
    FILE *fp;
    char line[N];

    if ((fp = fopen(filename, "rt")) == NULL) {
        fprintf(stdout, "Error creating file\n");
        exit(-3);
    }

    while (true) {
        while (fgets(line, N, fp) != NULL) {
            if (print) {
                fprintf(stdout, "%s", line);
            }
        }
        rewind(fp);
    }
}

void child() {
    // handler for SIGUSR2 & error check
    if(signal(SIGUSR2, child_manager) == SIG_ERR){
        printf("Signal Handler Error.\n");
        exit(1);
    }

    switch(fork()) {
        case 0:
            // child
            sleep(SEC);
            kill(getppid(), SIGUSR2);
            exit(0);

        case -1:
            // error
            fprintf(stdout, "Error, fork error\n");
            exit(-5);

        default:
            break;
    }

    // code
    while(true) {
        int rand = getRandomNumber((unsigned int) time(NULL), 1, 10);
        sleep((unsigned int) rand);
        kill(getppid(), SIGUSR1);
    }
}

/* SIGNAL MANAGER */
void parent_manager(int signal) {
    switch (signal) {
        case SIGUSR1:
            fprintf(stdout, "> (PARENT) Received SIGUSER1!\n");
            print = !print;
            break;

        case SIGUSR2:
            fprintf(stdout, "> (PARENT) Received SIGUSER2!\n");
            exit(0);

        default:
            fprintf(stderr, "Caught wrong signal: %d\n", signal);
            return;
    }
}

void child_manager(int signal) {
    switch (signal) {
        case SIGUSR2:
            fprintf(stdout, "\t> (CHILD) Received SIGUSER2!\n");
            // child_1 receives SIGUSER2 from child_2 and forward it to parent
            kill(getppid(), SIGUSR2);
            exit(0);

        default:
            fprintf(stderr, "Caught wrong signal: %d\n", signal);
            break;
    }
}

int getRandomNumber(unsigned int seed, int min, int max) {
    srand(seed);
    int num = rand() % (max - min + 1) + min;
    return num;
}
