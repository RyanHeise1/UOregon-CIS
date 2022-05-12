'''
CIS 122 Winter 2020 Lab 5
Author: Ryan Heise
Partner: None
Description: find the Factorial of a inputed number
'''

import math



def factorial(num):
    """tests the factorial without the .factorial

This function will take in a number entered in user(). After this it will run
a for loop to find the factorial

Args:
   num(int): number entered in user()

Returns:
   returns the factorial of the num entered
"""
    factorial = 1
    for i in range(1, num + 1):
        factorial = factorial * i
    print (factorial)



"""def test_factorial(num):
    tests the factorial entered

This function will accpet the number entered in the user function
it will run throught with the .factorial method and if there is an 
error it will raise the error count

Args:
   num(int): number entered in user()

Returns:
   returns the answer and the ammount of errors in my verson of finding
   a factorial.

    errors = 0 
    range_num = num + 1
    for i in range(range_num):
        user_factorial = factorial(i)
        math_factorial = math.factorial(i)
        if math_factorial != user_factorial:
            errors = errors + 1
        print(i, ":", user_factorial , math_factorial)
    print("Errors (", num, "): ", errors)
"""


def user():
    """asks for user input

This function will ask for you to enter a factorial number
if that number is < 0 it wil, print ann error else it will continue

Args:
   none

Returns:
   returns the number to test factorial
"""
    number = int(input("Enter factorial number: "))
    if number < 0:
        print("The Number You Entered Has To be Greater Than Or Equal To 0")
    elif number == 0:
        print("The Number 0 is equal to 1")
    else:
        factorial(number)

# test_factorial(4)
# factorial(4)

user()