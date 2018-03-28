/**
 * Name:    Francesco
 * Surname: Longo
 * ID:      223428
 * Lab:     2
 * Ex:      2
 *
 * Implement a C program, thread_generation_tree, which receives a command line parameter: n.
 * The main thread creates two other “children” threads and terminates. Each child thread creates another
 * two children threads, and terminates.
 * Thread creation stops after 2n leave threads have been created.
 * For example, if n=3, the main thread creates two children, and each child creates another two
 * threads for a total number of 8 leaves threads. At this point process creation stops.
 * Each leaf thread must print its generation tree, i.e., the sequence of thread identifiers from the
 * main thread.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define N 1024
#define M 100

int n;

// arguments for thread
typedef struct {
    int n;
    char generation_tree[N];
} Box;

void *thread_code(void *box_passed);
void eraseString(char *string, int size);

int main(int argc, char **argv) {
    fprintf(stdout, "> Start\n");

    int rc1, rc2;
    pthread_t thread1, thread2;
    Box box1, box2;
    char temp[M];

    if (argc != 2) {
        fprintf(stdout, "Expected 2 argument: <prog_name> <n>\n");
        exit(-1);
    }

    n = atoi(argv[1]);

    fprintf(stdout, "> (MAIN: %lu) - n: %d\n", pthread_self(), n);

    eraseString(temp, M);
    n--;
    sprintf(temp, "Threads' tree: %lu", pthread_self());

    strcpy(box1.generation_tree, temp);
    box1.n = n;

    strcpy(box2.generation_tree, temp);
    box2.n = n;

    if ( (rc1 = pthread_create(&thread1, NULL, thread_code, (void *) &box1)) != 0 ) {
        // error
        perror("\n - (e) Error creating thread1!");
        exit(-1);
    }

    if ( (rc2 = pthread_create(&thread2, NULL, thread_code, (void *) &box2)) != 0 ) {
        // error
        perror("\n - (e) Error creating thread2!");
        exit(-1);
    }

    //fprintf(stdout, "(%lu) n: %d - %lu, %lu\n", pthread_self(), n+1, thread1, thread2);

    pthread_exit(0);
}

void *thread_code(void *box_passed) {
    Box *box, box1, box2;
    int rc1, rc2;
    pthread_t thread1, thread2;
    char temp[N];

    box = (Box *) box_passed;
    int n = box->n;
    strcpy(temp, box->generation_tree);

    // end recursion
    if (n == 0) {
        // print path
        fprintf(stdout, "%s %lu\n", temp, pthread_self());

        // terminate
        pthread_exit(0);
    } else if (n > 0) {
        char temp1[N];

        // save new path
        sprintf(temp1, "%s %lu", temp, pthread_self());

        // decrement n
        n--;

        // save new box
        strcat(box1.generation_tree, temp1);
        box1.n = n;

        strcat(box2.generation_tree, temp1);
        box2.n = n;

        // reiterate
        if ( (rc1 = pthread_create(&thread1, NULL, thread_code, (void *) &box1)) != 0 ) {
            // error
            perror("\n - (e) Error creating thread1!");
            exit(-1);
        }

        if ( (rc2 = pthread_create(&thread2, NULL, thread_code, (void *) &box2)) != 0 ) {
            // error
            perror("\n - (e) Error creating thread2!");
            exit(-1);
        }

        // terminate
        pthread_exit(0);
    } else {
        // error
        fprintf(stdout, "Shouldn't be here -> ERROR\n");

        // terminate
        pthread_exit(0);
    }
}

void eraseString(char *string, int size) {
    for (int i=0; i<size; i++) {
        string[i] = (char) '\0';
    }
}
