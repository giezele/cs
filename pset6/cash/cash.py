# Implement a program that calculates the minimum number of coins required to give a user change.

from cs50 import get_float

# prompt for non-negative value
change = get_float("Change owed: ")
while True:
    if change <= 0:
        change = get_float("Change owed: ")

    # round dollar to cents
    cents = round(change * 100)

    # computing fewest amount of coins (25,10,5,1)
    coins = 0

    # how many quarters fits in cents amount
    while (cents >= 25):
        cents = cents - 25
        coins += 1

    # how many 10's fits in cents amount
    while (cents >= 10):
        cents = cents - 10
        coins += 1

    # how many 5's fits in cents amount
    while (cents >= 5):
        cents = cents - 5
        coins += 1

    # how many 1's fits in cents amount
    while (cents >= 1):
        cents = cents - 1
        coins += 1

    print(coins)
    break

