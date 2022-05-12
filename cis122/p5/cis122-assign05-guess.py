'''
CIS 122 Winter 2020 Assignment 5
Author: Ryan Heise
Partner: None
Description: Guessing word game!
'''

# User input for the word
word = str(input("Enter Guess Word (blank to quit): "))
new_word = "".join(set(word))
# if the word is blank then end the code
if word == "":
    exit()

def guess():
    tries = 0
    # take length of the new word
    len_word = len(new_word)
    # storing all the correct answers
    right_store = ""
    # storing all the wrong answers

    wrong_store = ""

    # Infinate loop
    while True:
        #look asking user for letter
        guess = input("Enter a guess letter (blank to quit): ")
        # if the guess is blank exit while loop
        if guess == "":
            break
        # if the guess is greater than one char then it will print an error
        if len(guess) > 1:
            print("\t> Only enter a single letter")
        # if the guess is already stored in the correct letters it will print already guessed
        elif guess in right_store:
            print("\t>", guess, "already guessed and found")
            print("\t> Guesses so far: " + right_store + wrong_store)
            tries = tries + 1
        # if the guess is already stored in the wrong letters it will print already guessed and not found
        elif guess in wrong_store:
            print("\t>", guess, "already guessed and not found")
            print("\t> Guesses so far: " + right_store + wrong_store)
            tries = tries + 1
        # if the guess is not in the word it will will say that the guess is not in word and add it to the wrong storage
        elif guess not in new_word:
            wrong_store = wrong_store + guess
            print("\t>", guess, "not found")
            print("\t> Guesses so far: " + right_store + wrong_store)
            tries = tries + 1
        else:
            # if the guess is in the word it will print that it is found and add it to the right storage
            right_store = right_store + guess
            if guess in new_word:
                print("\t>", guess, "found")
                print("\t> Guesses so far: ", right_store + wrong_store)
                tries = tries + 1
                len_word -= 1
        # when the length of the word = 0 will will print the results
        if len_word == 0:
            print("\t> All letters found\n")
            print("*** Results ***")
            print("Word: \t\t", word)
            print("Mathced: \t", right_store)
            print("Unmatched: \t", wrong_store)
            print("Guesses: \t", tries)
            break

guess()