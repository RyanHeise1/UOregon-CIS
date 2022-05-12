'''
CIS 122 Winter 2020 Lab 6
Author: Ryan Heise
Partner: None
Description: Average test score
Source: https://www.geeksforgeeks.org/max-min-python/ (helped me figure out how to find the lowest and hughest scores)
'''

# storing all the user test scores
test_score = []
# counting how many scores entered
count = 0
# adding up all the scores entered
total_score = 0

# creating an infinate loop
while True:
    # user input
    score = input("Enter Score: ")
    # if the inout is blank break out of the loop
    if score == "":
        break
    # changing score to an int
    score = int(score)
    # put the scores entered into test_score
    test_score.append(score)
    # add one to the counter
    count += 1
    # adding up the total score
    total_score += score

# find the average
average = total_score / count

# print the results
print("*** Results ***")
print("Count:", count)
print("Average:", average)
print("Low score:", min(test_score))
print("High score:", max(test_score))
