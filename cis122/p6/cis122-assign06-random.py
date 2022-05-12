'''
CIS 122 Winter 2020 Assignment 6
Author: Ryan Heise
Partner: Jayden Ben
Description: test to see if there is a file in your direstory. If so, it will add all numbers and return a value
'''

import os
from cis122_assign06_shared import pad_left, pad_right


# variables for the while loop
total = 0
comment_counter = 0
average = 0
count = 0
# spacing for final answer
spacing = 10
num_space = 7

# infinate loop
while True:
    # user input for filename
    filename = input("Enter filename (blank to exit): ")
    # if the input is blank then it will break the loop and exit code
    if filename == "":
        break  
    # if the file is in a directory
    if os.path.isfile(filename):
        # opens filemane as file
        with open(filename) as file:
            # for every line in that file it will find the comments and update the comment counter
            for line in file:
                if line.find("#") == 0:
                    comment_counter += 1
                # if there are no comments will will cac the total of all the lines and find the average
                else:
                    total += int(line)
                    count += 1
                    average = total / count
        # closing the file
        file.close()
        # printing results
        print()
        pad_right("Count: ", spacing + 1,  count)
        pad_right("Comments: ", spacing, comment_counter)
        pad_right("Total: ", spacing, total)
        pad_right("Average: ", num_space, average)
        print()
        # setting all the values back to 0 in clase the user calls another file
        total = 0
        comment_counter = 0
        count = 0
        average = 0
    # if file is not found print error with the name entered
    else:
        print("Invalid Filename: ", filename)