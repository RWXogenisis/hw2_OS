#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CHAIRS 5

int num_waiting = 0; // number of customers waiting
int barber_sleeping = 1; // whether the barber is sleeping or not

sem_t barber_sem; // semaphore to wake up the barber
sem_t customer_sem; // semaphore to signal a customer arrival
sem_t access_waiting_room; // semaphore to ensure mutual exclusion when accessing the waiting room
sem_t access_barber_chair; // semaphore to ensure mutual exclusion when accessing the barber chair

void* barber_thread(void* arg) {
    srand(time(0)); //generatig random seed
    while (1) {
        // Wait for a customer to arrive
        sem_wait(&customer_sem);
        
        // Acquire the waiting room and barber chair
        sem_wait(&access_waiting_room);
        sem_wait(&access_barber_chair);
        
        // Wake up the barber
        barber_sleeping = 0;
        
        // Serve the customer
        num_waiting--;
        printf("Barber: served customer, chairs available: %d\n", NUM_CHAIRS - num_waiting);
        usleep(rand() % 1000000); //waiting for a random time
        
        // Release the waiting room and barber chair
        sem_post(&access_barber_chair);
        sem_post(&access_waiting_room);
        
        // If there are no more customers, go to sleep
        if (num_waiting == 0) {
            barber_sleeping = 1;
        }
    }
}

void* customer_thread(void* arg) {
    int id = *((int*) arg);
    usleep(rand() % 1000000); // wait for a random amount of time
    
    // Try to enter the waiting room
    sem_wait(&access_waiting_room);
    
    // If there's a free chair, sit down and signal the barber
    if (num_waiting < NUM_CHAIRS) {
        num_waiting++;
        printf("Customer %d: waiting, chairs available: %d\n", id, NUM_CHAIRS - num_waiting);
        sem_post(&customer_sem);
        sem_post(&access_waiting_room);
        
        // Wait for the barber to finish cutting hair
        sem_wait(&access_barber_chair);
        
        // Get a haircut
        printf("Customer %d: got a haircut\n", id);
      sem_post(&access_barber_chair); //releasing the semaphore as it will be required by barber thread for next customer
    }
    // If there are no free chairs, leave the shop
    else {
        printf("Customer %d: left the shop, chairs available: %d\n", id, NUM_CHAIRS - num_waiting);
        sem_post(&access_waiting_room);
    }
    
    pthread_exit(NULL);
}

int main() {
    // Initialize the semaphores
    sem_init(&barber_sem, 0, 0);
    sem_init(&customer_sem, 0, 0);
    sem_init(&access_waiting_room, 0, 1); //initializing value to 1 so that any one of them can calim it first
    sem_init(&access_barber_chair, 0, 1);
    
    // Create the barber thread
    pthread_t barber;
    pthread_create(&barber, NULL, barber_thread, NULL);
    
    // Create 10 customer threads
    pthread_t customers[10];
    int customer_ids[10];
    for (int i = 0; i < 10; i++) {
        customer_ids[i] = i;
        pthread_create(&customers[i], NULL, customer_thread, (void*) &customer_ids[i]);
        //usleep(rand() % 1000000); // wait for a random amount of time
    }
    
    // Wait for the threads
    pthread_join(barber, NULL);
    for (int i = 0; i < 10; i++) {
        pthread_join(customers[i], NULL);
        //usleep(rand() % 1000000);
    }
    // Destroy semaphores
    sem_destroy(&barber_sem);
    sem_destroy(&customer_sem);
    sem_destroy(&access_waiting_room);
    sem_destroy(&access_barber_chair);
    return 0;
}

/*
The reason why the number of seats are not updating is because the updation occurs only when the barber informs that his service is complete
  and the reason why every process is getting a haircut before the service is complete is because the barber_chair is empty, hence they are allowed to take a seat.
*/
/*
OUTPUT:
~/hw2OS$ gcc barber.c -o barber
./~/hw2OS$ ./barber
Customer 2: waiting, chairs available: 4
Customer 2: got a haircut
Barber: served customer, chairs available: 5
Customer 0: waiting, chairs available: 4
Customer 5: waiting, chairs available: 3
Customer 5: got a haircut
Customer 7: waiting, chairs available: 2
Customer 7: got a haircut
Customer 3: waiting, chairs available: 1
Customer 3: got a haircut
Customer 1: waiting, chairs available: 0
Customer 1: got a haircut
Customer 6: left the shop, chairs available: 0
Customer 9: left the shop, chairs available: 0
Barber: served customer, chairs available: 1
Customer 8: waiting, chairs available: 0
Customer 8: got a haircut
Barber: served customer, chairs available: 1
Barber: served customer, chairs available: 2
Customer 4: waiting, chairs available: 1
Customer 4: got a haircut
Customer 0: got a haircut
Barber: served customer, chairs available: 2
Barber: served customer, chairs available: 3
Barber: served customer, chairs available: 4
Barber: served customer, chairs available: 5
*/
