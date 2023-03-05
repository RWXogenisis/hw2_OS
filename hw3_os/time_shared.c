#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define SHARED_MEM_SIZE sizeof(struct timeval)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./time <command>\n");
        exit(1);
    }

    // Create a region of shared memory to store the starting time
    void *shared_mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Fork a child process to run the specified command
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process: write the starting time to shared memory and exec the command
        struct timeval start_time;
        gettimeofday(&start_time, NULL);
        memcpy(shared_mem, &start_time, sizeof(start_time));
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    } else {
        // Parent process: wait for the child to terminate and read the starting time from shared memory
        int status;
        //waitpid(pid, &status, 0);
        wait(NULL);
        // if (WIFEXITED(status)) {
        // Child exited normally
        struct timeval start_time;
        memcpy(&start_time, shared_mem, sizeof(start_time));
        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
        printf("Elapsed time: %f\n", elapsed_time);
        // } else {
        //     // Child exited abnormally
        //     printf("Child process exited abnormally.\n");
        // }
    }

    // Release the shared memory
    munmap(shared_mem, SHARED_MEM_SIZE);

    return 0;
}
