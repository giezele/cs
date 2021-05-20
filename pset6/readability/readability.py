# Implement a program that computes the approximate grade level needed to comprehend some text

from cs50 import get_string


def main():
    # prompt user input
    text = get_string("Text: ")

    # func for counting letters
    total_letters = count_letters(text)

    # func for counting words
    total_words = count_words(text)

    # func for counting sentences
    total_sentences = count_sentences(text)

    # computing grade level according to Coleman-Liau index formula (index = 0.0588 * L - 0.296 * S - 15.8)
    # L = avg number of letters per 100 words, S = avg number of sentences per 100 words

    L = total_letters / total_words * 100
    S = total_sentences / total_words * 100
    index = 0.0588 * L - 0.296 * S - 15.8

    grade = round(index)
    print(grade)

    # printing grades
    if (grade < 1):
        print("Before Grade 1")
    elif (grade >= 16):
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


def count_letters(text):
    # count number of all letters in the text
    letter = 0
    for i in range(len(text)):
        if text[i].isalpha():
            letter += 1
    return letter


def count_words(text):
    # count number of all words in the text
    # starting from 1 not 0 (zodziu sakiny visada vienu daugiau nei tarpu)
    word = 1
    for i in range(len(text)):
        if text[i].isspace():
            word += 1
    return word


def count_sentences(text):
    # count number of all sentences in the text. Presume sentences only ends "." "?" "!"
    sentence = 0
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '?' or text[i] == '!':
            sentence += 1
    return sentence


if __name__ == "__main__":
    main()