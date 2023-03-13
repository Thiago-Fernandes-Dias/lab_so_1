#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define MAXITENS 8

/* variaveis globais */
int buffer[MAXITENS];
int pointer = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* gera um numero aleatorio de itens para produzir / consumir */
int nitens()
{
        sleep(1);
        return (rand() % MAXITENS);
}

/* insere item */
void insere_item(int item)
{
        buffer[pointer] = item;
        printf("\nitem %d inserido (valor=%d)", pointer, item);
        fflush(stdout);
        pointer++;
}

/* remove item */
int remove_item()
{
        pointer--;
        printf("\nitem %d removido (valor=%d)", pointer, buffer[pointer]);
        fflush(stdout);
}

/* produtor */
void *produtor(void *in)
{
        int i, k;
        printf("\nProdutor iniciando...");
        fflush(stdout);
        while (1)
        {
                pthread_mutex_lock(&mutex);
                /* produz */
                k = nitens();
                for (i = 0; i < k; i++)
                {
                        if (pointer == MAXITENS)
                        { /* buffer cheio -- aguarda */
                                printf("\nBuffer cheio -- produtor aguardando...");
                                fflush(stdout);
                                pthread_cond_wait(&cond, &mutex);
                                printf("\nProdutor reiniciando...");
                                fflush(stdout);
                        }
                        insere_item(rand() % 100);
                }
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
        }
}

/* consumidor */
void *consumidor(void *in)
{
        int i, k;
        printf("\nConsumidor iniciando...");
        fflush(stdout);
        while (1)
        {
                pthread_mutex_lock(&mutex);
                /* consome */
                k = nitens();
                for (i = 0; i < k; i++)
                {
                        if (pointer == 0)
                        { /* buffer vazio */
                                printf("\nBuffer vazio -- consumidor aguardando...");
                                fflush(stdout);
                                pthread_cond_wait(&cond, &mutex);
                                printf("\nConsumidor reiniciando...");
                                fflush(stdout);
                        }
                        remove_item();
                }
                pthread_cond_signal(&cond);
                pthread_mutex_unlock(&mutex);
        }
}

int main()
{
        pthread_t thread1, thread2;
        pthread_create(&thread2, NULL, &consumidor, NULL);
        pthread_create(&thread1, NULL, &produtor, NULL);
        pthread_join(thread1, NULL);
        exit(0);
}
