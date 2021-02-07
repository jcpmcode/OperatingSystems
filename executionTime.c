// Author: Jose Carlos Paniagua
// Date: January 25, 2021

#include <stdio.h>
#include <time.h>   // for clock_t, clock(), CLOCKS_PER_SEC
#include <unistd.h> // for sleep()

int main()
{
    // Variabels initialization...
    double time_spent = 0.0;

    // Initial time
    clock_t begin = clock();
    printf("Measuring time...\n\n");

    int a = 1 + 1;
    int b = 1 + 1;
    int c = 1 + 1;
    int d = 1 + 1;
    int e = 1 + 1;
    int f = 1 + 1;
    int g = 1 + 1;
    int res = a + b + c + d + e + f + g;

    printf("Result: %d\n\n", res);

    // Final time
    clock_t end = clock();

    // Calculate difference between end and begin times to get the time elapsed.
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time elpased is %f seconds\n\n", time_spent);

    return 0;
}