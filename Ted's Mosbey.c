#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Declare semaphores
sem_t sem1, sem2, sem3, sem4, sem5;

// Thread functions
void* print_part1() {
    sem_wait(&sem1); // Wait for sem1 to be available
    printf("I am happy");
    sem_post(&sem2); // Signal sem2 to allow the next thread to proceed
    return NULL;
}

void* print_part2() {
    sem_wait(&sem2); // Wait for sem2 to be available
    printf(" to be the students");
    sem_post(&sem3); // Signal sem3 to allow the next thread to proceed
    return NULL;
}

void* print_part3() {
    sem_wait(&sem3); // Wait for sem3 to be available
    printf(" of the department of");
    sem_post(&sem4); // Signal sem4 to allow the next thread to proceed
    return NULL;
}

void* print_part4() {
    sem_wait(&sem4); // Wait for sem4 to be available
    printf(" CSE EWU");
    sem_post(&sem5); // Signal sem5 to allow the next thread to proceed
    return NULL;
}

void* print_spaces() {
    sem_wait(&sem5); // Wait for sem5 to be available
    printf("\n");
    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&sem1, 0, 1); // Start with sem1 available
    sem_init(&sem2, 0, 0); // Other semaphores are initially unavailable
    sem_init(&sem3, 0, 0);
    sem_init(&sem4, 0, 0);
    sem_init(&sem5, 0, 0);

    // Create thread identifiers
    pthread_t t1, t2, t3, t4, t5;

    // Create threads
    pthread_create(&t1, NULL, print_part1, NULL);
    pthread_create(&t2, NULL, print_part2, NULL);
    pthread_create(&t3, NULL, print_part3, NULL);
    pthread_create(&t4, NULL, print_part4, NULL);
    pthread_create(&t5, NULL, print_spaces, NULL);

    // Wait for threads to complete
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);

    // Destroy semaphores
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
    sem_destroy(&sem4);
    sem_destroy(&sem5);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Global variables to hold input numbers and result
int num1, num2, result;
int choice;

// Semaphores for synchronization
sem_t sem_input_done, sem_choice_done, sem_result_ready;

// Thread functions
void* input_thread_func() {
    printf("Enter two numbers: ");
    scanf("%d %d", &num1, &num2);
    sem_post(&sem_input_done); // Signal that input is done
    return NULL;
}

void* choice_thread_func() {
    sem_wait(&sem_input_done); // Wait until input is done
    printf("Enter 1 for subtraction, 2 for multiplication: ");
    scanf("%d", &choice);
    sem_post(&sem_choice_done); // Signal that choice is made
    return NULL;
}

void* result_thread_func() {
    sem_wait(&sem_choice_done); // Wait until choice is made
    if (choice == 1) {
        result = num1 - num2;
    } else if (choice == 2) {
        result = num1 * num2;
    } else {
        printf("Invalid choice\n");
        exit(1);
    }
    sem_post(&sem_result_ready); // Signal that result is ready
    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&sem_input_done, 0, 0);
    sem_init(&sem_choice_done, 0, 0);
    sem_init(&sem_result_ready, 0, 0);

    // Create a child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) { // Child process
        pthread_t input_thread, choice_thread, result_thread;

        // Create threads
        pthread_create(&input_thread, NULL, input_thread_func, NULL);
        pthread_create(&choice_thread, NULL, choice_thread_func, NULL);
        pthread_create(&result_thread, NULL, result_thread_func, NULL);

        // Wait for threads to complete
        pthread_join(input_thread, NULL);
        pthread_join(choice_thread, NULL);
        pthread_join(result_thread, NULL);

        // Display the result
        sem_wait(&sem_result_ready); // Wait until result is ready
        if (choice == 1) {
            printf("The result of subtraction is: %d\n", result);
        } else if (choice == 2) {
            printf("The result of multiplication is: %d\n", result);
        }

        // Exit the child process
        exit(0);
    } else { // Parent process
        // Wait for the child process to complete
        wait(NULL);
        printf("All of my operations have been performed successfully.\n");
    }

    // Destroy semaphores
    sem_destroy(&sem_input_done);
    sem_destroy(&sem_choice_done);
    sem_destroy(&sem_result_ready);

    return 0;
}

