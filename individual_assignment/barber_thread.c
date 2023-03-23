#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CHAIRS 5 // Number of chairs in the waiting room

sem_t barber_sem; // Semaphore to wake up the barber
sem_t customer_sem; // Semaphore to signal a customer arrival
sem_t access_waiting_room; // Semaphore to access the waiting room
sem_t access_barber_chair; // Semaphore to access the barber chair

int num_waiting = 0; // Number of customers waiting
int barber_sleeping = 1; // Whether the barber is sleeping

void *barber_thread(void *arg)
{
    while (1) {
        // Wait for a customer to arrive
        sem_wait(&customer_sem);
        // Acquire the waiting room and barber chair
        sem_wait(&access_waiting_room);
        sem_wait(&access_barber_chair);
        // Barber is now awake
        barber_sleeping = 0;
        // Serve the customer
        printf("Barber is cutting hair\n");
        // Release the waiting room and barber chair
        sem_post(&access_waiting_room);
        sem_post(&access_barber_chair);
        // Go back to sleep if no customers are waiting
        barber_sleeping = 1;
    }
    return NULL;
}

void *customer_thread(void *arg)
{
    int id = *(int*)arg;
    // Try to enter the waiting room
    sem_wait(&access_waiting_room);
    if (num_waiting < NUM_CHAIRS) {
        // There is a free chair in the waiting room
        num_waiting++;
        printf("Customer %d is waiting in the waiting room\n", id);
        sem_post(&customer_sem);
        sem_post(&access_waiting_room);
        // Wait for the barber to finish cutting hair
        sem_wait(&access_barber_chair);
        // Get a haircut
        printf("Customer %d is getting a haircut\n", id);
        sem_post(&access_barber_chair);
    } else {
        // The waiting room is full, leave the shop
        printf("Customer %d is leaving the shop\n", id);
        sem_post(&access_waiting_room);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t barber;
    pthread_t customers[10];
    int ids[10];
    int i;
    // Initialize semaphores
    sem_init(&barber_sem, 0, 0); // the semaphore name, if shared between threads(0) or process(1), intial value
    sem_init(&customer_sem, 0, 0);
    sem_init(&access_waiting_room, 0, 1);
    sem_init(&access_barber_chair, 0, 1);
    // Start the barber thread
    //creating a thread for barber, default attributes for the thread, function to be called, arguments to be passed
    pthread_create(&barber, NULL, barber_thread, NULL); 
    // Start the customer threads
    for (i = 0; i < 10; i++) {
        ids[i] = i;
        //here we are passing the customer number to the function
        pthread_create(&customers[i], NULL, customer_thread, &ids[i]);
        // Sleep for a random time before the next customer arrives
        usleep(rand() % 1000000);
    }
    // Wait for the threads to finish
    pthread_join(barber, NULL);
    for (i = 0; i < 10; i++) {
        pthread_join(customers[i], NULL);
    }
    // Destroy semaphores
    sem_destroy(&barber_sem);
    sem_destroy(&customer_sem);
    sem_destroy(&access_waiting_room);
    sem_destroy(&access_barber_chair);
    return 0;
}
