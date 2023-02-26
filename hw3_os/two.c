#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Allocate space on the heap for an int
    int* ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    // Write something to the allocated memory
    *ptr = 42;
    
    // Spawn a child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        printf("Child process: %d\n", *ptr);
    } else {
        // Parent process
        printf("Parent process: %d\n", *ptr);
        // Wait for the child to finish
        wait(NULL);
    }
    
    // Free the allocated memory
    free(ptr);
    
    return 0;
}


/*
1. The program starts by allocating space on the heap for an int using the malloc function. If the allocation fails (i.e., returns NULL), an error message is printed using perror and the program exits with a failure status.
2. The program then writes the value 42 to the allocated memory using the dereferencing operator *.
3. The program spawns a child process using the fork function. The pid variable contains the process ID of the child process in the parent process, and it contains 0 in the child process.
4. If the fork function fails, an error message is printed using perror and the program exits with a failure status.
5. If the pid variable is 0, the code in the else if (pid == 0) block is executed in the child process. Otherwise, the code in the else block is executed in the parent process.
6. In both cases, the program prints the value of the allocated memory to the terminal using printf.
7. In the parent process, the program waits for the child process to finish using the wait function.
8. Fially, the program frees the allocated memory using the free function and returns 0.
*/

/*
