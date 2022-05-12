'''
CIS 122 Winter 2020 assignment 2
Author: Ryan Heise
Partner: None
Description: Create two functions to calculate a missing side using the Pythagorean Theorem.
'''
    
import math

# Question 1

# Calculate missing side c
def calc_side_c ( a, b ):
    # Take in side a and b and return c
    return round(math.sqrt((a * a) + (b * b)), 2)

# Calculate missing side a or b
def calc_side_ab ( ab, c ):
    # Use pythagoreans theorm to find side a/b
    return round(math.sqrt(math.pow(c, 2) - math.pow(ab, 2)), 2)

result1 = calc_side_c(5, 10)
result2 = calc_side_ab(4, 8)

print()
print("c = ", result1)
print("a/b = ", result2)
print()