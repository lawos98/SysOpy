#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

int santaIsSleeping;
int presentsCounter;
int reindeersWaiting;
int elvesWaiting;
int elves[10];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_ready = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t reindeers_busy = PTHREAD_COND_INITIALIZER;

void help_elves() {
    printf("Santa help elves : ");
    for (int i = 0; i < 10; i++) {
        if (elves[i] == 1) {
            printf(" %d ", i + 1);
            elves[i] = 0;
        }
    }
    printf("\n");
}

void santa() {
    while (true) {
        pthread_mutex_lock(&mutex);
        if (reindeersWaiting < 9 && elvesWaiting < 3) {
            santaIsSleeping = 1;
            pthread_cond_wait(&santa_ready, &mutex);
        }
        printf("Santa wake up Reindeers Waiting: %d , Elves Waiting: %d \n", reindeersWaiting, elvesWaiting);
        santaIsSleeping = 0;

        if (reindeersWaiting >= 9) {
            printf("Santa send presents (%d / 3)\n", presentsCounter + 1);
            reindeersWaiting = 0;
            presentsCounter += 1;
            sleep(rand() % 3 + 2);
            printf("Santa go sleep\n");
            pthread_cond_broadcast(&reindeers_busy);
        }
        if (elvesWaiting >= 3) {
            help_elves();
            elvesWaiting = 0;
            sleep(rand() % 2 + 1);
            printf("Santa go sleep\n");
            pthread_cond_broadcast(&queue_space);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void reindeer(int *id) {
    sleep(rand() % 6 + 5);
    while (true) {
        pthread_mutex_lock(&mutex);
        reindeersWaiting += 1;
        if (reindeersWaiting == 9) {
            printf("Reindeer %d wake up santa \n", *id);
            pthread_cond_broadcast(&santa_ready);
        } else {
            printf("Reindeer %d wating for Santa (all reideers wating: %d)\n", *id, reindeersWaiting);
        }
        pthread_cond_wait(&reindeers_busy, &mutex);
        pthread_mutex_unlock(&mutex);
    }
}

void elv(int *id) {
    int index = *id;
    index -= 1;

    while (true) {
        pthread_mutex_lock(&mutex);
        if (elvesWaiting == 3) {
            printf("Elv %d waiting for others 3 \n", *id);
            pthread_cond_wait(&queue_space, &mutex);
            while (elvesWaiting == 3) {
                pthread_cond_wait(&queue_space, &mutex);
            }
        }
        if (elvesWaiting < 3) {
            elvesWaiting += 1;
            elves[index] = 1;
            if (elvesWaiting < 3) {
                printf("Elv %d waiting for Santa (all elves waiting: %d)\n", *id, elvesWaiting);
            } else if (elvesWaiting == 3) {
                printf("Elv %d wake Santa \n", *id);
                pthread_cond_broadcast(&santa_ready);
            }
            pthread_cond_wait(&queue_space, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 4 + 2);
    }
}

int main() {
    srand(time(NULL));
    santaIsSleeping = 0;
    presentsCounter = 0;
    reindeersWaiting = 0;
    elvesWaiting = 0;

    pthread_t santa_thread;
    pthread_create(&santa_thread, NULL, (void *(*)(void *)) santa, NULL);

    pthread_t *elves_th = calloc(10, sizeof(pthread_t));
    int *ids_elves = calloc(10, sizeof(int));
    for (int i = 0; i < 10; i++) {
        ids_elves[i] = i + 1;
        pthread_create(&elves_th[i], NULL, (void *(*)(void *)) elv, ids_elves + i);
    }

    pthread_t *reindeers = calloc(9, sizeof(pthread_t));
    int *ids_rein = calloc(10, sizeof(int));
    for (int i = 0; i < 9; i++) {
        ids_rein[i] = i + 1;
        pthread_create(&reindeers[i], NULL, (void *(*)(void *)) reindeer, ids_rein + i);
    }

    while (presentsCounter != 3) {}
    printf("Santa send all Presents \n");
    return 0;
}