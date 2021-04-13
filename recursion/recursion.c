#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//2 options to count factorial:
//1 -- recursion
//2 -- iteration

double count_factorial1(double n);
double count_factorial2(double n);

int main(void)
{
    double number = get_int("number: ");
    double factorial1 = count_factorial1(number);
    printf("count_factorial1: %f\n", factorial1);
    double factorial2 = count_factorial2(number);
    printf("count_factorial2: %f\n", factorial2);

}

double count_factorial1(double n)
{
    if (n == 1)
        return 1;
    else
        return n * count_factorial1(n-1);
}

double count_factorial2(double n)
{
    double product = 1;
    while(n > 0)
    {
        product *= n;
        n--;
    }
    return product;
}