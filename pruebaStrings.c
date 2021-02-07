#include <stdio.h>

int main()
{
    char **arr;
    int i = 0;

    
    sprintf(arr, "hola");
    sprintf(arr, "pepe");

    do{
        printf("%s", arr[i]);
        i++;
    }
    while (i<2);

    return 0;
}