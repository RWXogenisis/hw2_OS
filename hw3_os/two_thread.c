#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>

// Function executed by the thread
void* thread_func(void* arg) {
    int* ptr = (int*) arg;
    printf("Thread: %d\n", *ptr);
    return NULL;
}

int main() {
    // Allocate space on the heap for an int
    int* ptr = malloc(sizeof(int));
    if (ptr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    // Write something to the allocated memory
    *ptr = 42;
    
    // Spawn a thread
    pthread_t thread;
    int rc = pthread_create(&thread, NULL, thread_func, ptr);
    if (rc != 0) {
        fprintf(stderr, "pthread_create: %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    // Wait for the thread to finish
    rc = pthread_join(thread, NULL);
    if (rc != 0) {
        fprintf(stderr, "pthread_join: %d\n", rc);
        exit(EXIT_FAILURE);
    }
    
    // Print the contents of the allocated memory in the main thread
    printf("Main thread: %d\n", *ptr);
    
    // Free the allocated memory
    free(ptr);
    
    return 0;
}

/*
1. The program starts by allocating space on the heap for an int using the malloc function, just like in the previous implementation using processes.
2. The program then writes the value 42 to the allocated memory using the dereferencing operator *, just like in the previous implementation using processes.
3. The program spawns a thread using the pthread_create function. The thread variable contains the thread ID of the newly created thread, and the thread_func function is executed by the thread. The arg parameter is passed to the thread_func function as an argument, and it contains a pointer to the allocated memory.
4. If the pthread_create function fails, an error message is printed using fprintf and the program exits with a failure status.
5. The program waits for the thread to finish using the pthread_join function. This function blocks the main thread until the specified thread terminates. If the pthread_join function fails, an error message is printed using fprintf and the program exits with a failure status.
6. The program prints the value of the allocated memory to the terminal in the main thread using printf.
7. Finally, the program frees the allocated memory using the free function and returns 0, just like in the previous implementation using processes.
*/
