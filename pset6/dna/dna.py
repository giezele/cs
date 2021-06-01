# implement a program that identifies to whom a sequence of DNA belongs

# open CSV file and DNA sequence, read contents into memory

# for each STR, compute the longest run of consecutive repeats in the DNA:
# for each position in the sequence: compute how many times the STR repeats starting at that position
# for each position keep checking succesive substrings until the STR repeats no longer - len(s)

# compare the STR counts against each row in the CSV file

import csv
import sys


def main():
    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py DATABASE.csv SEQUENCE.txt")

    # TODO: Read names into memory from file
    # with csv.reader read names from CSV file
    db_file = sys.argv[1]
    with open(db_file, 'r') as file:
        reader = csv.reader(file)
        # next() moves pointer to next line and returns first line- next allways returns 1 line!
        all_substrings = next(reader)[1:]
        # print(all_substrings)

        text_file = sys.argv[2]
        with open(text_file, 'r') as file:
            dna_string = file.read()
            # make arr for numbers counted in function calculate_STR_max_count() - sequence to match for
            person_in_txt = [calculate_STR_max_count(dna_string, sub) for sub in all_substrings]
            # print(person_in_txt)

        print_match(reader, person_in_txt)


def calculate_STR_max_count(dna_string, substring):
    # TODO compute individual counts of STR - longest repeats in the DNA

    # initialize arr of zeroes in len(dna_string) and turn every 0 into 1 at every slot where substring starts
    # increase 1 if substring repeats
    count = [0] * len(dna_string)

    # loop from end of dna_string to 0 sizeof(substring) steps decreasing i by -1
    for i in range(len(dna_string) - len(substring), -1, -1):  # for(int i = strlen(s)-strlen(sub); i > -1; i--) loop from end
        # s[i:j] = slice of string from i to j is substring
        if dna_string[i: i + len(substring)] == substring:
            # check if it is the base case
            if i + len(substring) > len(dna_string) - 1:
                count[i] = 1
            else:
                count[i] = 1 + count[i + len(substring)]

    return max(count)


def print_match(reader, person_in_txt):
    # compare arr of person_in_text against each row in the CSV file
    for line in reader:
        name = line[0]
        values = [int(val) for val in line[1:]]
        if values == person_in_txt:
            print(name)
            return
    print("No match")


if __name__ == "__main__":
    main()