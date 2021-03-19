#include <cs50.h>
#include <stdio.h>
#include <math.h>


int main(void)
{
    float change;
    //prompt for input only float non-negative
    do
    {
        change = get_float("Change owed: ");
    }
    while (change <= 0);
    
    //round dollar to cents and then cast float to int for math-precision
    int cents = (int) round(change * 100);
    
    //computing fewest amount of coins (25,10,5,1)
    int coins = 0;
    
    //how many quarters fits in cents amount
    while (cents >= 25)
    {
        cents = cents - 25;
        coins++;
    }
    
    //how many dimes fits in leftover cents amount
    while (cents >= 10)
    {
        cents = cents - 10;
        coins++;
    }
    
    //how many 5's fits in leftover cents amount
    while (cents >= 5)
    {
        cents = cents - 5;
        coins++;
    }
    
    //how many 1's fits in leftover cents amount
    while (cents >= 1)
    {
        cents = cents - 1;
        coins++;
    }
    
    printf("%i\n", coins);


}