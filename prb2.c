#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define LOW 2
#define HIGH 9

int random_gen(){
    srand(time(NULL));
    int temp = 0;
    do{
    temp = rand()%((HIGH-LOW+1)+LOW);
    }while(temp==0);
    return temp;
}

int john(int value, int n){
    if(n>=value){ //checking of the other player has reached the target

        value = value*random_gen();
        printf("John process %d\n", value);

        if(n<value){ //checking if the current player has reached the target
            printf("John won the game\n");
            return (-100);
        }
        else{
            return value;
        }
    }
    else{
        return (-100);
    }
}

int michael(int value, int n){
    if(n>=value){
        value*=random_gen();
        printf("Michael process %d\n", value);

        if(n<value){ //checking if after multiplication of the player has reached the target
            printf("Michael won the game\n");
            return (-100);
        }
        else{
            return value;
        }
    }

    else{ //in case the other player has already reached the target
        return (-100);
    }
}

int main(){
    int n;
    scanf("%d", &n);
    int p = 1;
    int x = 1;
    int chance = 1;
    int p1[2]; //C=>P
    int p2[2]; //P=>C

    //in case the pipes are not declared
    if (pipe(p1)==-1){
        return -1;
    }
    if(pipe(p2) == -1){
        return -10;
    }

    while(x>0 && p>0){ // run the loop till there is a winner, -100 indicates there is a winner
        int pid = fork();

        switch(pid){
            case -1:
                return -2;
                break;

            case 0:
                sleep(2); //this is to allow the parent process to execute completely and pass the value to pipe
                // close(0);
                // close(p2[1]);
                // dup(p2[0]);
                close(p1[0]); //closing the unused sides of the pipe
                close(p2[1]);

                if(read(p2[0], &x, sizeof(x)) == -1){ //reading the updated value from pipe, returns -1 if there is invalid value
                    return -3;
                }

                if(x != -100){ //this is a flag where we check if there is a winner or not
                    printf("Received %d\n", x);
                    x = michael(x, n); //updating the value
                    chance+=1;
                    printf("value of x is %d\n", x);

                    if(write(p1[1], &x, sizeof(x)) == -1){
                        return -4;
                    }
                }
                printf("Sent %d\n", x);
                //close(p2[0]);
                //close(p1[1]);
                break;
            
            default:
            //parent process
                close(p1[1]); //closing the unused ends of the pipe
                close(p2[0]);

                if(chance != 1){
                    if(read(p1[0], &p, sizeof(p)) == -1){
                        return -5;
                    }
                }
                if(p != -100){ //checking for a clear winner
                    printf("parent received %d\n", p);

                    p = john(p, n);
                    chance+=1;
                    printf("value of p is %d\n", p);
                    if(write(p2[1], &p, sizeof(p)) == -1){  //writing the new value to the pipe
                        return -6;
                    }
                }
                //printf("%d\n", p2[1]);
                printf("parent sent %d\n", p);
                //close(p1[0]);
                //close(p2[1]);
                wait(NULL); //waiting for child process to run to maintain the order
                //sleep(3);
        }
    }
}

//there will be junk values printed at the end
