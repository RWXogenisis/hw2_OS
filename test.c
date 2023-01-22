#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void child(char* options[]){
    execvp(options[0], options);
}


int main(){
    char string[20];
    system("pwd");

    while(strcmp("q", string) != 0){
        
        printf("%c", '$');
        fgets(string, 20, stdin);        
        string[strcspn(string, "\n")] = '\0';

        char* token = strtok(string, " "); 
        char* options[10];
        int index = 0;
        
        while(token != NULL){
            options[index] = token;
            token = strtok(NULL, " "); 
            index++;
        }
        options[index] = NULL;

        pid_t pid;

        switch(pid=fork()){
            case -1:
                perror("fork failed");
                exit(EXIT_FAILURE);
                break;
            
            case 0:
                
                child(options);
                break;

            default:
                wait(NULL);
        
        }
        
    }
    return 0;
}
