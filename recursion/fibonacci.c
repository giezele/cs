#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// recursion with 2 base cases
// The Fibonacci number sequence is defined as follows:
// first element is 0
// second element is 1
// n^th element is the sum of (n-1) and (n-2)

double count_fibonacci(double n);

int main(void)
{
  double number;
  printf("Enter a positive number: ");
  scanf("%lf", &number);
  double fibonacci1 = count_fibonacci(number);
  printf("count fibonacci: %f\n", fibonacci1);

}

double count_fibonacci(double n)
{
  if (n == 1)
    return 0;
  else if (n == 2)
    return 1;
  else
    return count_fibonacci(n-1) + count_fibonacci(n-2);
}

