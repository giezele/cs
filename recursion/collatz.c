#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/*
The Collatz conjecture applies to positive integers. It is always possible to get "back to 1" if you follow these steps:
- if n is 1, stop.
- otherwise, if n is even, repeat this process on n/2.
- otherwise, if n is odd, repeat this process on 3n+1.

Write a recursive function collatz(n) that calculates how many steps it takes to get back to 1 if you start from n and recurse as indicated above.
*/

int count_collatz(int n);

int main(void)
{
  int number;
  printf("Enter a positive number: ");
  scanf("%i", &number);
  int collatz1 = count_collatz(number);
  printf("count collatz: %i\n", collatz1);

}

int count_collatz(int n)
{
  //base case
  if (n == 1)
    return 0;

  //even numbers
  else if ((n % 2) == 0)
    return 1 + count_collatz(n/2);

  //odd numbers
  else 
    return 1 + count_collatz(3*n + 1);
}
