#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int startSize;
    int endSize;

    // TODO: Prompt for start size
    do
    {
        startSize = get_int("Start size: ");
    }
    while (startSize < 9);

    // TODO: Prompt for end size
    do
    {
        endSize = get_int("End size: ");
    }
    while (endSize < startSize);

    // TODO: Calculate number of years until we reach threshold
    int currentSize = startSize;
    int years = 0;
    if (currentSize < endSize)
    {
        do
        {
            currentSize += (currentSize / 3) - (currentSize / 4);
            years++;
        }
        while (currentSize < endSize);
    }
    
    // TODO: Print number of years
    printf("Years: %i \n", years);
}
