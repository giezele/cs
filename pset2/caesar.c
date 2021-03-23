#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Caesar.c
 * Write a program that enables you to encrypt messages using Caesar’s cipher. 
 * At the time the user executes the program, they should decide, 
 * by providing a command-line argument, on what the key should be in the secret 
 * message they’ll provide at runtime. We shouldn’t necessarily assume that the user’s key 
 * is going to be a number; though you may assume that, if it is a number, 
 * it will be a positive integer, if not - return from main a value of 1.
 * 
 * */

//check if user inputs program name and positive int, aka key of cipher
//if no print error msg and return 1
//check return status in CLI (echo $?)

int validate_key(string key);

int main(int argc, string argv[])
{
    if (argc != 2 || validate_key(argv[1]) != 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //if key validates as all-digits cast it into int
    int cipher = atoi(argv[1]);

    //prompt user input
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");

    //turning plaintext to ciphertext by adding cipher to every letter if it is alphabetical
    string ciphertext;

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]) && islower(plaintext[i]))
        {
            //convert ASCII to alphabetical index (a=0, b=1, c=2...)
            //shift adding a cipher and dividing by %26 to stay in the loop of a-z
            //convert back into ASCII
            int cipherchar = (((plaintext[i] - 'a') + cipher) % 26) + 'a';
            printf("%c", cipherchar);
        }
        else if (isalpha(plaintext[i]) && isupper(plaintext[i]))
        {
            int cipherchar = (((plaintext[i] - 'A') + cipher) % 26) + 'A';
            printf("%c", cipherchar);
        }
        else
        {
            int cipherchar = plaintext[i];
            printf("%c", cipherchar);
        }
    }
    printf("\n");
    return 0;
}


// checks if every elem of string is numeric
int validate_key(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isdigit(key[i]))
        {
            return 1;
        }
    }
    return 0;
}