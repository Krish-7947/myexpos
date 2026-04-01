#include <stdio.h>

int main()
{
    FILE *f = fopen("numbers.dat", "w");

    for (int i = 1; i <= 2047; i++)
    {
        fprintf(f, "%d\n", i);
    }

    fclose(f);
    return 0;
}