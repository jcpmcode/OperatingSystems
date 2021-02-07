/**
 * @file pthreads4.c
 * @author Jose Carlos (jcpmcode@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_HILOS 5

// Variables compartidas por los hilos
int dato = 0; /* dato compartido por los hilos */
int flag[2];  /* 0 es falso y 1 es verdadero */
int turn;

sem_t s;

void *resta(); /* hilo 1 */
void *suma();  /* hilo 2 */

void *asigna(void *arg)
{
  int par = *(int *)arg;

  sem_wait(&s);

  // Seccion crítica
  int t = dato;

  printf("Hola desde el hilo %d\n", par);

  sleep(1);

  t = t * par;

  dato = t;
  printf("Adios desde el hilo %d\n", par);

  // da paso a semaforo
  sem_post(&s);

  // Termina
  pthread_exit(0);
}

int main(int argc, char *argv[])
{
  pthread_t tid[NUM_HILOS]; /* the thread identifier */
  pthread_attr_t attr;      /* set of attributes for the thread */
  int datos[NUM_HILOS];

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <integer value>\n", argv[0]);
    /*exit(1);*/
    return -1;
  }
  if (atoi(argv[1]) < 0)
  {
    fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
    /*exit(1);*/
    return -1;
  }

  /* El parametro que recibo es el valor inicial de dato */
  dato = atoi(argv[1]);

  sem_init(&s, 0, 1); // Permite entrar a 1 proceso

  /* get the default attributes */
  pthread_attr_init(&attr);

  /* crea los hilos */
  for (int i = 0; i < NUM_HILOS; i++)
  {
    datos[i] = i;
    pthread_create(&tid[i], &attr, asigna, &datos[i]);
  }
  /* espera a los hilos */
  for (int i = 0; i < NUM_HILOS; i++)
  {
    pthread_join(tid[i], NULL);
  }
  printf("dato = %d\n", dato);
}