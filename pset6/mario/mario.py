# Implement a program that prints out a half-pyramid of a specified height
# first prompt the user with get_int for the half-pyramid’s height, a positive integer between 1 and 8, inclusive

from cs50 import get_int

# prompt for number 1-8 inclusively
n = get_int("Height: ")
while True:
    if n < 1 or n > 8:
        n = get_int("Height: ")
    
    for i in range(n):
        print(" " * (n - 1 - i), end="")
        print("#" * (i + 1))
    break
