#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


sem_t forks[5];
pthread_t philosophers[5];

void think(int philosopher_id) {
    printf("%d is THINKING\n", philosopher_id);
    sleep(1);
}

void eat(int philosopher_id) {
    printf("%d is EATING\n", philosopher_id);
    sleep(1);
}

void *philosopher(void* arg) {
    int id = *((int *)arg);

    while (1) {
        think(id);

        int left_fork = id;
        int right_fork = (id+1)%5;

        if (id % 2 == 0) {
            sem_wait(&forks[left_fork]);
            sem_wait(&forks[right_fork]);
        } else {
            sem_wait(&forks[right_fork]);
            sem_wait(&forks[left_fork]);
        }

        eat(id);

        sem_post(&forks[left_fork]);
        sem_post(&forks[right_fork]);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int philosopher_id[5];

    for (int i=0; i<5;i++) {
        sem_init(&forks[i], 0, 1);
        philosopher_id[i] = i;
    }

    for (int i=0; i<5;i++) {
        pthread_create(&philosophers[i], NULL, &philosopher, &philosopher_id[i]);
    }

    for (int i=0; i<5; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;

}
