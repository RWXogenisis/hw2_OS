#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define LOW 2
#define HIGH 9

int random_gen(){
    srand(time(NULL));
    int temp = rand()%((HIGH-LOW+1)+LOW);
    return temp;
}

int john(int value, int n){
    if(n>=value){

        value = value*random_gen();
        printf("John process %d\n", value);
        return value;
    }
}

int michael(int value, int n){
    if(n>=value){
        value*=random_gen();
        printf("Michael process %d\n", value);
        return value;
    }
}

int main(){
    int n;
    scanf("%d", &n);
    int p = 1;
    pid_t pid;
    int fd[2];
    pipe(fd);

    int chance = 1; 

    while(!(p>=n)){
        switch (pid = fork())
        {
        case -1:
            perror("fork failure");
            exit(EXIT_FAILURE);
            break;

        case 0:
            p = michael(p, n);
            chance += 1;
            close(fd[0]); 
            close(1);
            dup(fd[1]);
            write(fd[1], &p, sizeof(int));
            break;
        
        default:
            close(fd[1]); 
            close(0); 
            dup(fd[0]); 
            int n1 = read(fd[0], &p, sizeof(int));

            p = john(p, n);
            chance+=1;
            wait(NULL);
        }
    }

    if(chance%2 == 0){
        printf("John won\n");
    }
    else{
        printf("Michale won\n");
    }
    return 0;
}
