#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <time.h>

int *arr;

void bubbleSort(int a[], int length)
{
    int i, j, temp;
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length - i - 1; j++)
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

int main()
{
#define tam 100000

    arr = mmap(NULL, tam * sizeof(int), PROT_READ | PROT_WRITE,
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (int i = 0; i < tam; i++)
    {
        arr[i] = rand() % 100; // Entre 0 y 99
    }

    long inicio = clock();

    int pid = fork();
    if (pid > 0)
    {
        printf("hello\nfather here \n Ordene ");
        bubbleSort(arr, tam / 2);
        wait(&pid);
        mezcla(arr, 0, tam / 2, tam - 1);
        long fin = clock();
        double t = (fin - inicio) / ((double)CLOCKS_PER_SEC);
        printf("Tiempo transcurrido: %f\n\n", t);
    }
    else
    {
        //arr[0] = 20;
        printf("hello\nchild here \n Ordene");
        bubbleSort(&arr[tam / 2], tam / 2);
    }

    return 0;
}