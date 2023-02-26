#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILENAME "info.txt"

int main() {
    // Open the input file for reading
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    // Create the first child process
    pid_t c1_pid = fork();
    if (c1_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (c1_pid == 0) {
        // Child 1 reads only the odd-numbered lines from the file
        char line[256];
        int line_num = 1;
        while (fgets(line, sizeof(line), fp) != NULL) {
            if (line_num % 2 == 1) {
                printf("Child 1: %s\n", line);
            }
            line_num++;
        }
        // Child 1 exits
        exit(EXIT_SUCCESS);
    }

    
    // Create the second child process
    pid_t c2_pid = fork();
    rewind(fp);
    if (c2_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (c2_pid == 0) {
        // Child 2 reads only the even-numbered lines from the file
        char line[256];
        int line_num = 1;
        while (fgets(line, sizeof(line), fp) != NULL) {
            if (line_num % 2 == 0) {
                printf("Child 2: %s\n", line);
            }
            line_num++;
        }
        // Child 2 exits
        exit(EXIT_SUCCESS);
    }
    fclose(fp);
    
    // Parent process waits for both child processes to finish
    int status;
    waitpid(c1_pid, &status, 0);
    waitpid(c2_pid, &status, 0);
    
    // Open the output file for appending
    fp = fopen(FILENAME, "a");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    // Append a random integer to each line of the file
    rewind(fp);
    // char line[256];
    // while (fgets(line, sizeof(line), fp) != NULL) {
        int rand_num = rand() % 100;
        fprintf(fp, " %d\n", rand_num);
    // }
    
    // Close the file and exit
    fclose(fp);
    return 0;
}

/*
1. The program starts by opening the input file "info.txt" for reading using the fopen function. If the file cannot be opened, an error message is printed using perror and the program exits with a failure status.
2. The program then creates two child processes, C1 and C2, using the fork function. If fork returns -1, an error message is printed using perror and the program exits with a failure status.
3. Child 1 reads only the odd-numbered lines from the file using a loop and the fgets function. If the line number is odd, the line is printed to the terminal using printf. The child process then exits with a success status using exit.
4. Child 2 reads only the even-numbered lines from the file using a loop and the fgets function. If the line number is even, the line is printed to the terminal using printf. The child process then exits with a success status using `exit
*/
