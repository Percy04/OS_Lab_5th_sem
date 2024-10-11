#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

sem_t wrt, mutex;
int readcount;
int val=1;
int count=1;

void* w(void *);
void* r(void *);

void *w(void *arg) {
        count++;
        sem_wait(&wrt);
        val = count;
        printf("writing: %d\n", count);
        sem_post(&wrt);
}

void *r(void *arg) {
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1) {
            sem_wait(&wrt);
        }
        sem_post(&mutex);

        printf("Reading: %d\n", val);

        sem_wait(&mutex);
        readcount--;
        if (readcount == 0) {
            sem_post(&wrt);
        }
        sem_post(&mutex);
        return 0;
}

int main(int argc, char* argv[]) {
    sem_init(&wrt, 0, 1);
    sem_init(&mutex, 0, 1);

    pthread_t* reader, *writer;
    
    int numThreads = abs(atoi(argv[1]));
    reader = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);
    writer = (pthread_t *) malloc(sizeof(pthread_t) * numThreads);

    for (int i=0;i<numThreads;i++) {
        pthread_create(&reader[i], NULL, &r, NULL);
        pthread_create(&writer[i], NULL, &w, NULL);
    }

    for (int i=0; i<numThreads;i++) {
        pthread_join(writer[i], NULL);
        pthread_join(reader[i], NULL);
    }
    

}
