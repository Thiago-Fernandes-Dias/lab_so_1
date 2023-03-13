/*
 * Implementação da solução do livro do Tanenbaum
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define N 11
#define MAX_REFEICOES 100

#define ESQ (phil_id + 1) % N
#define DIR (phil_id + N - 1) % N

typedef enum {T, H, E} estado_t;
estado_t estado[N];

sem_t sem;
sem_t filosofo[N];

/* Contabilidade */
sem_t sem_ref;
int refeicoes[N], total_refeicoes;

/* Considera que já adquiriu o semaforo
   ou está no início da execução   */
void exibe_mesa() {
  int i;
  for (i = 0; i < N; i++)
    switch (estado[i]) {
    case T:  printf("T ");
      break;
    case H:  printf("H ");
      break;
    case E:  printf("E ");
      break;
    }
  printf("\n");
}

/* Considera que já adquiriu o semaforo */
void testa_garfos(int phil_id) {
  if (estado[phil_id] == H && estado[ESQ] != E && estado[DIR] != E) {
    estado[phil_id] = E;
    exibe_mesa();
    sem_post(&filosofo[phil_id]);
  }
}

void pensa(int phil_id) {
  sleep(1);
}

void pega_garfos(int phil_id) {
  sem_wait(&sem);
  estado[phil_id] = H;
  exibe_mesa();
  testa_garfos(phil_id);
  sem_post(&sem);
  sem_wait(&filosofo[phil_id]);
}


void come(int phil_id) {
  sem_wait(&sem_ref);
  refeicoes[phil_id]++;
  total_refeicoes++;
  if (total_refeicoes == MAX_REFEICOES) {
    int i;
    printf("Total de refeicoes: \n");
    for (i = 0; i < N; i++)
      printf("Filosofo %d: %d\n", i, refeicoes[i]);
    /* Aborta execução */
    exit(0);
  }
  sem_post(&sem_ref);
  sleep(1);
}

void solta_garfos(int phil_id) {
  sem_wait(&sem);
  estado[phil_id] = T;
  exibe_mesa();
  testa_garfos(ESQ);
  testa_garfos(DIR);
  sem_post(&sem);
}

void* f_phil(void *v) {
  int phil_id = *(int *) v;

  while(1) {

    pensa(phil_id);
    
    pega_garfos(phil_id);

    come(phil_id);
    
    solta_garfos(phil_id);
  }
}
      

int main() {
  pthread_t thr[N];
  int i, phil_id[N];

  sem_init(&sem, 0, 1);
  sem_init(&sem_ref, 0, 1);
  total_refeicoes = 0;
  for (i = 0; i < N; i++) {
    phil_id[i] = i;    
    sem_init(&filosofo[i], 0, 0);
    estado[i] = T;
    refeicoes[i] = 0;
  }
	     
  exibe_mesa();
  
  for (i = 0; i < N; i++) 
    pthread_create(&thr[i], NULL, f_phil, (void*) &phil_id[i]);

  for (i = 0; i < N; i++) 
    pthread_join(thr[i], NULL);
  
  return 0;
}
