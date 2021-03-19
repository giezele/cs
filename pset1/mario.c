#include <cs50.h>
#include <stdio.h>


int main(void)
{
    int n;
    //prompt for number 1-8 inclusively
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    //draw line
    for (int i = 0; i < n; i++) //this is hight
    {
        for (int j = 0; j < n - 1 - i; j++) //this is dots in a row
        {
            printf(" ");
        }
        for (int k = 0; k < i + 1; k++) //this is # in a row
        {
            printf("#");
        }
        
        printf("\n");
    }
}