'''
CIS 122 Winter 2020 Lab 4
Author: Ryan Heise
Partner: None
Description: Let's create a function that will take an integer number representing a month (e.g. the number 10 for October).
'''


# Define a fuction to see if number entered is a leap year
def is_leap_year(num):
    # If the remainder of the num is equal to 0, then it is flaso
    # ex: 7.5 would return false
    # ex: 7 would return true because there is no remainder... remainder = 0
    if num % 4 == 0:
        # If 
        if num % 100 == 0:
            # If num is divisible by 4 and 100 and the remainder is 0 then its true
            if num % 400 == 0:
                # Return true to user() fcn
                return True
            else:
                # Return false to user() fcn
                return False
        else:
            # Return true to user() fcn
            return True
    else:
        # Return false to user() fcn
        return False



# Defined function to add user input
def user():
    # declare variable "num" for the initial user input
    # /n starts user input on next line
    num = int(input("Enter a Year to Figure Out If It is a Leap Year: \n"))
    # calling the function is_leap_year and defining it as leap_year
    leap_year = is_leap_year(num)
    # If num entered produces "true" then it runs
    if leap_year == True:
        print(num, "is a leap year")
    else:
        # If num is false this runs
        print(num, "is not a leap year")


# Calling "user" function to start code and prompt user for input
user()