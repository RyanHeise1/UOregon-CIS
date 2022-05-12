'''
CIS 122 Winter 2020 assignment 2
Author: Ryan Heise
Partner: None
Description: Create two functions to help calculate the amount of cement in yards, and to display the results.
'''

"""
A cement slab 4" thick, 60" wide and 120" long requires 0.62 yards of cement
A cement slab 4" thick, 240" wide and 240" long requires 4.94 yards of cement

" = inches

"""
import math

# Return cement amount in yards using cubic inches given thickness (t), width (w) and length (l) in inches
def calc_yards_cement ( t, w, l):
    # Convert length and width to feet
    length = l / 12
    width = w / 12
    # Calc total cubic yards
    cubic_yards = round((length * width * (t/12)) / 27, 2)
    return cubic_yards

# Output (print) results of calculating yards given thickness (t), width (w) and length (l) in inches
def print_results ( t, w, l, result):
    print("A cement slab", t, "thick,", w, "wide and", l, "long requires", result) 


print()
result1 = calc_yards_cement(4,60,120)
print_results(4, 60, 120, result1)
result2 = calc_yards_cement(4, 240, 240)
print_results(4, 240, 240, result2)
print()

