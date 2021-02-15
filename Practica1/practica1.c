/**
 * @file practica1.c
 * @author  Jose Carlos Paniagua
 * @brief Práctica 1: Ordenamiento y Criba de Eratóstenes
 * @version 0.1
 * @date 2021-02-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *arr; /* tam compartido por los hilos */
int tam = 0;
void *mitad1(); /* hilo 1 */
void *mitad2(); /* hilo 2 */

void bubbleSort(int a[], int length)
{
  int i, j, temp;
  for (i = 0; i < length; i++)
  {
    for (j = 0; j < length - i; j++)
    {
      if (a[j + 1] < a[j])
      {
        temp = a[j];
        a[j] = a[j + 1];
        a[j + 1] = temp;
      }
    }
  }
}

void imp(int arr[], int len)
{
  int i;

  printf("Arreglo ordenado:\n");
  for (i = 0; i < len; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void mezcla(int arr[], int ini, int med, int fin)
{
  int *aux = malloc(sizeof(int) * (fin - ini + 1));
  int finIni = med - 1;
  int s = ini;
  int tam = fin - ini + 1;

  int t = 0;
  while ((ini <= finIni) && (med <= fin))
  {
    if (arr[ini] <= arr[med])
    {
      aux[t] = arr[ini];
      ini++;
      t++;
    }
    else
    {
      aux[t] = arr[med];
      med++;
      t++;
    }
  }
  while (ini <= finIni)
  {
    aux[t++] = arr[ini++];
  }

  while (med <= fin)
  {
    aux[t++] = arr[med++];
  }

  for (int i = 0; i <= (tam); i++)
  {
    arr[s + i] = aux[i];
  }
}

void ordenamiento()
{
  pthread_t tid1, tid2; /* the thread identifier */
  pthread_attr_t attr;  /* set of attributes for the thread */

  printf("Ingresa el tamaño del arreglo a ordenar: ");
  scanf("%d", &tam);

  arr = malloc(tam * sizeof(int));
  printf("\n\nArreglo aleatorio:\n");
  for (int i = 0; i < tam; i++)
  {
    arr[i] = rand() % tam; // Entre 0 y 99
    printf("%d ", arr[i]);
  }
  printf("\n\n");

  long begin = clock();

  /* get the default attributes */
  pthread_attr_init(&attr);

  /* create the thread 1*/
  pthread_create(&tid1, &attr, mitad1, NULL);

  /* create the thread 2*/
  pthread_create(&tid2, &attr, mitad2, NULL);

  /* espera a los hilos */
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  mezcla(arr, 0, tam / 2, tam - 1);

  // Final time
  long end = clock();
  double t = end-begin;
  t = t / ((double)CLOCKS_PER_SEC);

  imp(arr, tam);
  free(arr);
  printf("\nTiempo transcurrido: %f segundos", t);
}

void criba()
{
  int n;

  printf("Ingresa un valor: ");
  scanf("%d", &n);
  fflush(stdin);
  int primos[n + 1];
  if (n < 2)
  {
    printf("Los números primos empiezan desde el 2.");
  }
  else
  {
    // Llenamos el arreglo hasta el número indicado
    for (int i = 1; i <= n; i++)
    {
      primos[i] = i;
    }
    // Recorremos el ciclo aplicando la criba de Eratóstenes
    for (int i = 2; i * i <= n; i++)
    {
      if (primos[i] != -1)
      {
        // Marcamos como -1 los números que no son primos
        for (int j = 2 * i; j <= n; j += i)
          primos[j] = -1;
      }
    }
    printf("Los números primos son: \n");
    for (int i = 2; i <= n; i++)
    {
      if (primos[i] != -1)
      {
        printf("%d ", i);
      }
    }
  }
}

int main()
{
  char str[256];

  while (1)
  {
    fflush(stdin);
    printf("\n\n¿Qué operación deseas realizar?\n");
    printf("\t1. Ordenamiento\n");
    printf("\t2. Criba de Eratóstenes\n");
    printf("\t3. Salir\n");
    fgets(str, sizeof(str), stdin);
    fflush(stdin);
    switch (atoi(str))
    {
    case 1:
      printf("\n\nOrdenamiento\n\n");
      ordenamiento();
      break;
    case 2:
      printf("\n\nCriba de Eratóstenes\n\n");
      criba();
      break;
    case 3:
      printf("\n\n¡Adiós!\n\n");
      exit(0);
    default:
      printf("\n\nEsa no es una opción valida. Por favor ingresa una opción del menú:");
    }
  }

  return 0;
}

void *mitad1()
{
  bubbleSort(arr, tam / 2);
  pthread_exit(0);
}

void *mitad2()
{
  bubbleSort(&arr[tam / 2], tam / 2);
  pthread_exit(0);
}