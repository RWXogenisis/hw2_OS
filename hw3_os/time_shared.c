#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char* argv[]) {
    int pipefd[2];
    pid_t pid;
    struct timeval start, end;
    double elapsed_time;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // child process
        gettimeofday(&start, NULL);  // record start time
        close(pipefd[0]);  // close unused read end of pipe
        dup(pipefd[1]);  // redirect stdout to write end of pipe
        close(pipefd[1]);  // close write end of pipe
        execvp(argv[1], &argv[1]);  // execute command
        perror("execvp");  // execvp only returns if there's an error
        exit(EXIT_FAILURE);
    } else {  // parent process
        close(pipefd[1]);  // close unused write end of pipe
        wait(NULL);  // wait for child to finish
        read(pipefd[0], &start, sizeof(start));  // read start time from pipe
        gettimeofday(&end, NULL);  // record end time
        close(pipefd[0]);  // close read end of pipe
        elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0;  // calculate elapsed time in milliseconds
        elapsed_time += (end.tv_usec - start.tv_usec) / 1000.0;
        printf("Elapsed time: %.2f ms\n", elapsed_time/1000.0);
    }

    return 0;
}
