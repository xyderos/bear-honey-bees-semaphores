#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <cstdlib>
#include <cstdio>
#include <semaphore.h>
#define HONEYBEES 10
#define CAPACITY 10

sem_t isFull, isGathering;

int bees, cap, pot;

void *bear(void *arg){

    for(;;){

        sem_wait(&isFull);

        printf("BEAR IS WOKE AND EATING FROM THE POT, THE POT IS NOW EMPTY.\n");

        pot = 0;

        sem_post(&isGathering);
    }
}

void *honeyBees(void *arg){

    for(;;){

        sem_wait(&isGathering);

        pot++;

        printf("HONEYBEE %ld GATHERED HONEY, POT IS NOW %d\n",(long)arg,pot);

        if(pot == cap) sem_post(&isFull);

        else sem_post(&isGathering);
    }
}

int main(int argc, char *argv[]){

    pthread_attr_t attr;

    pthread_t workerid[HONEYBEES + 1];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    sem_init(&isFull, 0, 0);

    sem_init(&isGathering, 0, 1);

    cap = (argc > 2) ? atoi(argv[2]) : CAPACITY;

    bees = (argc > 1)? atoi(argv[1]) : HONEYBEES;

    long i = 0;

    pthread_create(&workerid[0],NULL ,bear, (void *) i);

    for(i = 1; i <= bees; i++) pthread_create(&workerid[i],NULL, honeyBees, (void *) i);

    for(i = 1; i<= bees; i++)pthread_join(workerid[i], NULL);
}
