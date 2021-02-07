#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *arr; /* dato compartido por los hilos */
int dato = 0;
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

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2; /* the thread identifier */
    pthread_attr_t attr;  /* set of attributes for the thread */

    if (argc != 2)
    {
        fprintf(stderr, "usage: a.out <integer value>\n");
        return -1;
    }

    if (atoi(argv[1]) < 0)
    {
        fprintf(stderr, "Argument %d must be non-negative\n", atoi(argv[1]));
        return -1;
    }

    dato = atoi(argv[1]);
    printf("Tamaño: %d\n\n", dato);
    arr = malloc(dato * sizeof(int));
    for (int i = 0; i < dato; i++)
    {
        arr[i] = rand() % dato; // Entre 0 y 99
        printf("%d ", arr[i]);
    }
    printf("\n\n");
    // long inicio = clock();

    /* get the default attributes */
    pthread_attr_init(&attr);

    /* create the thread 1*/
    pthread_create(&tid1, &attr, mitad1, NULL);

    /* create the thread 2*/
    pthread_create(&tid2, &attr, mitad2, NULL);

    /* espera a los hilos */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    mezcla(arr, 0, dato / 2, dato - 1);

    imp(arr, dato);
    free(arr);
    return 0;
}
/**
 * Este hilo mitad1
 */
void *mitad1()
{
    bubbleSort(arr, dato / 2);
    pthread_exit(0);
}
/**
 * Este hilo mitad2
 */
void *mitad2()
{
    bubbleSort(&arr[dato / 2], dato / 2);
    pthread_exit(0);
}
