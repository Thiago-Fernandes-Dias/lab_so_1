/*
 * Leitores e escritores utilizando locks e variáveis de condição.
 * Sem starvation.
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

volatile int bd = -1; /* Banco de dados */

volatile int nl = 0;

pthread_mutex_t lock_bd = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_nl = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *leitor(void* v) {
  int id = (int) v;

  pthread_mutex_lock(&lock_bd);
  pthread_mutex_lock(&lock_nl);
  nl++;
  pthread_mutex_unlock(&lock_nl);
  pthread_mutex_unlock(&lock_bd);
  sleep(1);
  printf("Leitor %d leu o valor %d \n", id, bd);
  pthread_mutex_lock(&lock_nl);
  nl--;
  if (nl == 0) 
    pthread_cond_signal(&cond);
  pthread_mutex_unlock(&lock_nl);
  return NULL;
}

void *escritor(void *v) {
  int id = (int) v;
  
  pthread_mutex_lock(&lock_bd);
  pthread_mutex_lock(&lock_nl);
  while (nl > 0) 
    pthread_cond_wait(&cond, &lock_nl);
  bd = id;
  sleep(1);
  printf("Escritor %d escreveu o valor %d \n", id, bd);
  pthread_mutex_unlock(&lock_nl);
  pthread_mutex_unlock(&lock_bd);

  return NULL;
}

#define N 60
int main() {
  pthread_t thr[N];
  int i = 0, l = 0, e = 0;

  for (i = 0; i < N; i++) {
    if (random() % 2) 
      pthread_create(&thr[i], NULL, leitor, (void*) l++);
    else
      pthread_create(&thr[i], NULL, escritor, (void*) e++);
  }

  pthread_exit(NULL);
}
