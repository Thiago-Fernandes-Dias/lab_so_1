/*
 * Leitores e escritores utilizando semáforos.
 * Possibilidade de leitores simultâneos, mas
 * também de starvation dos escritores.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NL 10
#define NE 3

sem_t sem_bd;
volatile int bd = -1; /* Banco de dados */

sem_t sem_nl;
volatile int nl = 0;

void *leitor(void* v) {
  int id = (int) v;

  sem_wait(&sem_nl);
  nl++;
  if (nl == 1)
    sem_wait(&sem_bd);
  sem_post(&sem_nl);
  printf("Leitor %d leu o valor %d \n", id, bd);
  sleep(1);
  sem_wait(&sem_nl);
  nl--;
  if (nl == 0)
    sem_post(&sem_bd);
  sem_post(&sem_nl);
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

#define N 300
int main() {
  pthread_t thr[N];
  int i = 0, l = 0, e = 0;

  sem_init(&sem_bd, 0, 1);
  sem_init(&sem_nl, 0, 1);
  
  for (i = 0; i < N; i++) {
    if (random() % 2) 
      pthread_create(&thr[i], NULL, leitor, (void*) l++);
    else
      pthread_create(&thr[i], NULL, escritor, (void*) e++);
  }

  pthread_exit(NULL);
}

