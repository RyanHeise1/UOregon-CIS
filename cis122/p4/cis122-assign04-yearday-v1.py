'''
CIS 122 Winter 2020 Assignment 4
Author: Ryan Heise
Partner: None
Description: create functions that asks the user for the year and the day of the year
'''


# Define a fuction to see if number entered is a leap year
def is_leap_year(year):
    # If the remainder of the num is equal to 0, then it is flaso
    # ex: 7.5 would return false
    # ex: 7 would return true because there is no remainder... remainder = 0
    if year % 4 == 0:
        # If 
        if year % 100 == 0:
            # If num is divisible by 4 and 100 and the remainder is 0 then its true
            if year % 400 == 0:
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



year = int(input("Enter Year: "))
if year < 0:
    print("Year Must Be Greater Than 0")


leap_year = is_leap_year(year)
"""if leap_year:
    days = 366
else:
    days = 365"""


day = int(input("Enter Day Of The Year: "))
if day < 0:
    print("Days In The Year Must Be Greater Than 0")
if is_leap_year(year) is True: 
    print("Day Must Be Less Then Or Equal To 366")
elif day == 1 or day <= 31:
    month = ("January")
elif day == 32 or day <= 60:
    month = ("Febuary")
    day = day - 31
elif day == 61 or day <= 91:
    month = ("March")
    day = day - 60
elif day == 92 or day <= 121:
    month = ("Arpil")
    day = day - 91
elif day == 122 or day <= 152:
    month = ("May")
    day = day - 121
elif day == 153 or day <= 182:
    month = ("June")
    day = day - 152
elif day == 183 or day <= 213:
    month = ("July")
    day = day - 182
elif day == 214 or day <= 244:
    month = ("August")
    day = day - 213
elif day == 245 or day <= 274:
    month = ("September")
    day = day - 244
elif day == 275 or day <= 305:
    month = ("October")
    day = day - 274
elif day == 306 or day <= 335:
    month = ("November")
    day = day - 305
elif day == 336 or day <= 366:
    month = ("December")
    day = day - 335
else:
    if day <= 0:
        print("Day Must Be Greater Than Or Equal To 0")
    elif day == 1 or day <= 31:
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
