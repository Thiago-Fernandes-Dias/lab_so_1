/*
 * Leitores e escritores utilizando semáforos.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int bd = -1; /* Banco de dados */
sem_t sem_bd;

void *leitor(void* v) {
  int id = (int) v;

  sem_wait(&sem_bd);
  sleep(1);
  printf("Leitor %d leu o valor %d \n", id, bd);
  sem_post(&sem_bd);
  
  return NULL;
}

void *escritor(void *v) {
  int id = (int) v;
  
  sem_wait(&sem_bd);
  bd = id;
  sleep(1);
  printf("Escritor %d escreveu o valor %d \n", id, bd);
  sem_post(&sem_bd);

  return NULL;
}

#define N 60
int main() {
  pthread_t thr[N];
  int i = 0, l = 0, e = 0;

  sem_init(&sem_bd, 0, 1);
  for (i = 0; i < N; i++) {
    if (random() % 2) 
      pthread_create(&thr[i], NULL, leitor, (void*) l++);
    else
      pthread_create(&thr[i], NULL, escritor, (void*) e++);
  }

  pthread_exit(NULL);
}

