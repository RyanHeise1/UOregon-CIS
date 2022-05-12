'''
CIS 122 Winter 2020 Assignment 3
Author: Ryan Heise
Partner: https://www.w3schools.com/python/ref_func_len.asp
Description: Write a fruitful function reverse() that accepts a string, and will return the string in reverse order
'''


def reverse(string):

    """One line description

Extended summary

Args:
   param1 (data type of param 1): description of param1

Returns:
   Description of return value (None if void function)
"""

    output = ""   
    for i in range(len(string)):
        output = output + string[len(string)-1-i] 
    return output 
  


string = "When in the Course of human events"
print("Original: ",string)
print("Reversed: " , reverse(string))