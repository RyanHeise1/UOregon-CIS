'''
CIS 122 Winter 2020 Assignment 4
Author: Ryan Heise
Partner: None
Description: create functions that asks the user for the year and the day of the year
'''



# function defined in lab 4 
def is_leap_year(year):
    """This function will calculate, if the year entered, is a leap year

   This function will take in the year:
   if the year is divisible by 4 then it will return true
   if year is divisible by 100 it will return Flase 
   if year is dviisible by 400 and 100 it is true

Args:
   year(int): the yeear you entered in input_year

   Returns:
      True/False
"""
# If the remainder of the num is equal to 0, then it is flaso
# ex: 7.5 would return false
# ex: 7 would return true because there is no remainder... remainder = 0
    if year % 4 == 0:
        # If 
        if year % 100 == 0:
            # If num is divisible by 4 and 100 and the remainder is 0 then its true
            if year % 400 == 0:
                # Return true to start() fcn
                return True
            else:
                # Return false to start() fcn
                return False
        else:
            # Return true to start() fcn
            return True
    else:
        # Return false to start() fcn
        return False
    return is_leap_year

# produces true/false if year is valid/invalid
# Need to use true/ false
def valid_year(year):
    """This Function will validate if the year is true or not and return true or false 

replaces year validity checks, printing out any error, and returning True if the year is valid, 
or False if the year is invalid.

Args:
   year(int): the yeear you entered in input_year

Returns:
   returns True or False 
"""
    if year > 0:
        return True
    else: 
        return False

# check if day is valid and produces true/false 
def valid_day_of_year(year, valid_day_of_year):
    """This function takes in the year and tests if it is valid

replaces day of year validity checks, printing out any error, and returning True 
if the day of year is valid, or False if the day of year is invalid

Args:
   year(int): the yeear you entered in input_year
   valid_day_of_year(bool): returns true or false

Returns:
   Description of return value (None if void function)
"""
    if year > 0:
        return True
    else:
        return False


# code that prompts user for the year
def input_year():
    """Asks user of year 

encapsulates the prompting for the year, and returns either 0 if the year is invalid, 
or the year as an integer.

Args:
   None... It's a user prompt

Returns:
   returns the year entered
"""
    year = int(input("Enter Year: "))
    if valid_year(year) is True:
        return year
    if year <= 0:
        print("The Year Entered Must Be Greater Than 0")


# prompt user of day of the year 
# I should be calling (valid_day_of_year) and (valid_year)
def input_day_of_year(year):
    """Asks user for day

encapsulates the prompting for the day of the year, and returns either 0 if the day of the year is invalid, 
or the day of the year as an integer. Note that the function includes a year parameter, so your function 
should also call both valid_day_of_year() and valid_year() functions.

Args:
   year(int): the yeear you entered in input_year

Returns:
   Returns day entered
"""
    day = int(input("Enter Day Of The Year: "))
    if day < 0:
        print("The Numebr Entered Has To Be Greater Than 0")
    else:
        return day


# returns 0 for invalid year 
# num of days as an int
def get_days_in_year(year):
    """ Will return the ammount of days in the year

if the year entered is a leap year it will return 366 if not 365

Args:
   year(int): the yeear you entered in input_year

Returns:
   returns 366 if a leap year and 365 if it is a normal year
"""
    if is_leap_year(year) is True:
        return 366
    if is_leap_year(year) is False:
        return 365
    else:
        return 0


# returns true if month is valid (ex: 0 < month <= 12)
# false for an invalid month ("Month must be > 0 " and "Month must be <= 12")
def valid_month(month):
    """returns true if month is valid (ex: 0 < month <= 12)

returns True if a month is valid (e.g. 0 < month <= 12), or False for an invalid month, 
with error output of "Month must be > 0" and "Month must be <= 12"

Args:
   month(int): Month that is declared in get_day_string

Returns:
   returns error and flase if moth is less than 0 or if month is greater than 12 and true if 
   it meetes requirements
"""
    if 0 < month <=12:
        return True
    elif month <= 0:
        print("The Month Entered Must Be Greater Than 0")
        return False
    elif month >= 12:
        print("The Month Entered Must Be Less Than Or Equal To Than 12")
        return False


# returns empty string for invalid month 
# call valid_month(month)
def translate_month(month):
    """takes in the valid month function and if it is true it will run

takes in the valid month function and if it is true it will run. if the month is = 1 it will return 
january and so on untill december

Args:
   month(int): Month that is declared in get_day_string

Returns:
   returns respected month for the month number 
   ex: 1 = january
"""
    if valid_month(month) is True:
        if month == 1:
            return "January"
        if month == 2:
            return "Febuary"
        elif month == 3:
            return "March"
        elif month == 4:
            return "April"
        elif month == 5:
            return "May"
        elif month == 6:
            return "June"
        elif month == 7:
            return "July"
        elif month == 8:
            return "August"
        elif month == 9:
            return "September"
        elif month == 10:
            return "October"
        elif month == 11:
            return "November"
        elif month == 12:
            return "December"
        


# returns num of days in a month as an int  
# need to account for leap years
# if it is a leap year + 1 days
def get_days_in_month(year,month):
    """if month is qual to 1 then it will return the respected mont

if month is qual to 1 then it will return the respected month. It also takes into account of Febuary. 
A month that gains one day if it is a leap year or not

Args:
   year(int): Year entered into input_year
   month(str): used to figure out the ammount of days needed for that month

Returns:
   returns the respective days in that month
"""
    if month == 1:
        return 31
    elif is_leap_year(year) is True and valid_month(month) is True and month == 2:
        return 29
    elif is_leap_year(year) is False and valid_month(month) is True and month == 2:
        return 28
    if month == 3:
        return 31
    if month == 4:
        return 30
    if month == 5:
        return 31
    if month == 6:
        return 30
    if month == 7:
        return 31
    if month == 8:
        return 31
    if month == 9:
        return 30
    if month == 10:
        return 31
    if month == 11:
        return 30
    if month == 12:
        return 31


# returns flase if the year,month, OR day is invalid 
def valid_day(year, month, day):
    """Checks if the day is valid

checks if day is valid. It call valid year to see if year is valid
valid month to check if month is valid
and valid day of the year

Args:
   year(int): year entered into inout year
   month(str): month used in translate month
   day(int): day entered in consol

Returns:
   returns true or false if everything checks out to be true
"""
    if valid_year(year) is True and valid_month(month) is True and valid_day_of_year(year,day) is True:
        return True
    else:
        return False 


# string formatted as Month Day, year (Ex: December 1, 2018)
# returns empty string if year,month, OR day is invalid
def get_date_string(year, month, day):
    """Takes in day entered and returns the respected month

that returns an empty string if the year, month or day are invalid, or a string formatted as Month Day, Year

Args:
   year(int): the yeear you entered in input_year
   month(int): Month that is declared in get_day_string
   day(int): day enterd in past function 

Returns:
   Description of return value (None if void function)
"""
    if is_leap_year(year) is True and valid_year(year) is True:
        if (day == 1) or (day <= 31):
            month = ("January")
            return 1
        elif day == 32 or day <= 60:
            month = ("Febuary")
            day = day - 31
            return 2
        elif day == 61 or day <= 91:
            month = ("March")
            day = day - 60
            return 3
        elif day == 92 or day <= 121:
            month = ("Arpil")
            day = day - 91
            return 4
        elif day == 122 or day <= 152:
            month = ("May")
            day = day - 121
            return 5
        elif day == 153 or day <= 182:
            month = ("June")
            day = day - 152
            return 6
        elif day == 183 or day <= 213:
            month = ("July")
            day = day - 182
            return 7
        elif day == 214 or day <= 244:
            month = ("August")
            day = day - 213
            return 8
        elif day == 245 or day <= 274:
            month = ("September")
            day = day - 244
            return 9
        elif day == 275 or day <= 305:
            month = ("October")
            day = day - 274
            return 10
        elif day == 306 or day <= 335:
            month = ("November")
            day = day - 305
            return 11
        elif day == 336 or day <= 366:
            month = ("December")
            day = day - 355
            return 12
        print(month, str(day) + ",", year)
    if is_leap_year(year) is False and valid_year(year) is True:
        if day == 1 or day <= 31:
            month = ("January")
        elif day == 32 or day <= 59:
            month = ("Febuary")
            day = day - 31
        elif day == 60 or day <= 90:
            month = ("March")
            day = day - 59
        elif day == 91 or day <= 120:
            month = ("April")
            day = day - 90
        elif day ==  121 or day <= 151:
            month = ("May")
            day = day - 120
        elif day == 152 or day <= 181:
            month = ("June")
            day = day - 151
        elif day == 182 or day <= 212:
            month = ("July")
            day = day - 181
        elif day == 213 or day <= 243:
            month = ("August")
            day = day - 212
        elif day == 244 or day <= 273:
            month = ("September")
            day = day - 243
        elif day == 274 or day <= 304:
            month = ("October")
            day = day - 273
        elif day == 305 or day <= 334:
            month = ("November")
            day = day - 304
        elif day == 335 or day <= 365:
            month = ("December")
            day = day - 334
        print(month,str(day) + ",", year)


def start():
    """Starts program

Program starts when variable is called 

Args:
    none

Returns:
   NONE
"""
    month = get_date_string
    year = input_year()
    get_date_string(year, month, input_day_of_year(year))


start()






































