/*
A barbershop consists of a waiting room with n chairs, and the
barber room containing the barber chair. If there are no customers
to be served, the barber goes to sleep. If a customer enters the
barbershop and all chairs are occupied, then the customer leaves
the shop. If the barber is busy, but chairs are available, then the
customer sits in one of the free chairs. If the barber is asleep, the
customer wakes up the barber. Write a program to coordinate the
barber and the customers.

*/

// there are n semaphores
// if the remaining semaphores are n then sleep
// if semaphores are between 0 to n then decrement the semaphore
// if a customer got his haircut leave and increment the semaphore
// if the semaphore is 0 then customer leaves as semaphore cannot be negative logic wise

#include <stdio.h>
#include <stdlib.h>

//creating the semaphore
typedef struct{
    int value;
    struct process *list;
}semaphore;

typedef struct queue{
    int value;
    struct queue* next;
}queue;

//global variables;
queue* order = NULL;
int count=0;

//balk??
void balk(){
    ;
}


void insert(){
    queue*temp = (queue*)malloc(sizeof(queue));
    count++;
    temp->value = count;
    temp->next = NULL;
    if(order == NULL){
        order = temp;
    }
    else{
        queue*temp1 = order;
        while(temp1->next!=NULL){
            temp1 = temp1->next;
        }
        temp1->next = temp;
    }
}


//wait function
int getHairCut(semaphore* S){
    if (S->value>0){
        if (S->value < 5){
            printf("\nThe customer is waiting at seat: %d", S->value);
            insert();
        }
        else if(S->value == 5){
            printf("\nWaking the barber up and the customer is having a haircut");
            insert();
        }
        S->value--;
        return 0;
    }
    else{
        printf("\nCustomer is leaving as there are no chairs");
        balk();
        return -1;
    }
}

// signal function
int barberDone(semaphore* S){
    if(S->value<5){
        S->value++;
        printf("\nCustomer number %d has finished his haircut", order->value);
        queue* temp = order;
        order = order->next;
        free(temp);
        return 0;
    }
    else{
        printf("\nInvalid call");
        return -1;
    }
}

int main(int argc, char const *argv[])
{
    semaphore* chairs;
    int temp=0;

    printf("Enter the number of chairs: ");
    scanf("%d", &chairs->value);

    
    // while(temp!=-1)
    //     temp = getHairCut(chairs);
    
    // temp = 0;
    // while(temp!=-1)
    //     temp = barberDone(chairs);

    getHairCut(chairs);
    getHairCut(chairs);
    barberDone(chairs);

    return 0;
}
