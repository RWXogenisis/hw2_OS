#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FILENAME "info.txt"

typedef struct {
    int start_line;
    int num_lines;
} thread_args;

void* red_odd(void* arg){
    FILE* fp = fopen(FILENAME, "r");
    char line[256];
    int line_num = 1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_num % 2 == 1) {
            printf("Thread %ld: %s", pthread_self(), line);
        }
        line_num++;
    }
    fclose(fp);
}

void* red_even(void* arg){
    FILE* fp = fopen(FILENAME, "r");
    char line[256];
    int line_num = 1;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line_num % 2 == 0) {
             printf("Thread %ld: %s", pthread_self(), line);
        }
        line_num++;
    }
    fclose(fp);
}
// void* read_file(void* arg) {
//     // Extract the thread arguments
//     thread_args* args = (thread_args*) arg;
//     int start_line = args->start_line;
//     int num_lines = args->num_lines;
    
//     // Open the file for reading
//     FILE* fp = fopen(FILENAME, "r");
//     if (fp == NULL) {
//         perror("fopen");
//         exit(EXIT_FAILURE);
//     }
    
//     // Read the lines from the file
//     char line[256];
//     int line_num = 1;
//     while (fgets(line, sizeof(line), fp) != NULL) {
//         if (line_num >= start_line && line_num < start_line + num_lines) {
//             printf("Thread %ld: %s", pthread_self(), line);
//         }
//         line_num++;
//     }
    
//     // Close the file and exit
//     fclose(fp);
//     return NULL;
// }

int main() {
    // Create the two threads
    pthread_t thread1, thread2;
    thread_args args1 = {1, 2};  // Read lines 1-2
    thread_args args2 = {2, 2};  // Read lines 3-4
    pthread_create(&thread1, NULL, red_even, NULL);
    pthread_create(&thread2, NULL, red_odd, NULL);
    
    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    // Open the file for appending
    FILE* fp = fopen(FILENAME, "a");
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
The program defines a struct thread_args to hold the arguments for each thread. The struct contains the starting line number and the number of lines to read from the file.
The program defines a function read_file that takes a pointer to a thread_args struct as its argument. The function opens the file for reading using fopen, reads the specified lines from the file using a loop and the fgets function, and prints the lines to the terminal using printf. The function then closes the file and exits with a NULL pointer.
In the main function, the program creates two threads using pthread_create. Each thread is passed a pointer to a thread_args struct that specifies which lines to read from the file. The threads execute the read_file function using the specified arguments.
The main function waits for both threads to finish using pthread_join.
The program then opens the file for appending using fopen. If the file cannot be opened, an error message is printed using perror and the program exits with a failure status.
The program appends a random integer to each line of the file using a loop and the fprintf function.
The program closes the file using fclose and exits with a success status.
*/
