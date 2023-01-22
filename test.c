#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void child(char* options[]){
    // can use execlp but need individual strings to mention the params
    //printf("%s\n", options[0]);
    execvp(options[0], options);
}


int main(){
    char string[20];
    system("pwd");

    while(strcmp("q", string) != 0){
        
        printf("%c", '$');
        fgets(string, 20, stdin);
        //removing the trailing newline charcater
        string[strcspn(string, "\n")] = '\0';

        char* token = strtok(string, " "); //tokenising the strings
        //char* command = token; //getting the command name
        char* options[10];
        int index = 0;
        
        while(token != NULL){
            //printf("%s\n", token);
            //getting the options
            options[index] = token;
            token = strtok(NULL, " "); // null is for tokeniszing the same string
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
                //child process
                child(options);
                break;

            default:
            //waiting for the child process to be excuted
                wait(NULL);
        
        }
        /*
        if(strcmp("q", string) != 0){
            printf("\n%s command....\n", string);
            system(string);
        }
        */
        
    }

    return 0;
}
