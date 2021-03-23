#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 * Implement a program that computes the approximate grade level needed to comprehend some text.
 * One such readability test is the Coleman-Liau index. 
 * Meaning, what (U.S.) grade level is needed to understand the text. The formula is: 
 * index = 0.0588 * L - 0.296 * S - 15.8
 * Here, L is the average number of letters per 100 words in the text, 
 * and S is the average number of sentences per 100 words in the text.
 * */

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    //prompt user input
    string text = get_string("Text: ");

    //func for counting letters
    int total_letters = count_letters(text);

    //func for counting words
    int total_words = count_words(text);

    //func for counting sentences
    int total_sentences = count_sentences(text);

    //computing grade level according to Coleman-Liau index formula (index = 0.0588 * L - 0.296 * S - 15.8)
    // L = avg number of letters per 100 words, S = avg number of sentences per 100 words

    float L = (float)total_letters / (float)total_words * 100.0;
    float S = (float)total_sentences / (float)total_words * 100.0;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    int grade = round(index);

    //printing grades
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }

}

int count_letters(string text)
{
    //count number of all letters in the text
    int letter = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letter++;
        }
    }
    return letter;
}

int count_words(string text)
{
    //count number of all words in the text
    //starting from 1 not 0 (zodziu sakiny visada vienu daugiau nei tarpu)
    int word = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isspace(text[i]))
        {
            word++;
        }
    }
    return word;
}

int count_sentences(string text)
{
    //count number of all sentences in the text. Presume sentences only ends "." "?" "!"
    int sentence = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if ((text[i]) == '.' || (text[i]) == '?' || (text[i]) == '!')
        {
            sentence++;
        }
    }
    return sentence;
}