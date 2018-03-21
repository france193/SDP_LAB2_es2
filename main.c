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

#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

int main(int argc, char **argv) {
    fprintf(stdout, "> Start\n");

    if (argc != 2) {
        fprintf(stdout, "Expected 3 argument: <prog_name> <n>\n");
        exit(-1);
    }

    return 0;
}
