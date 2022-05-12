'''
Title: Project 5b - Print Calendar

Author: Ryan Heise

Credit: N/A
'''

import datetime



month_list = ['January', 'February', 'March', 'April', 'May',
              'June', 'July', 'August', 'September', 'October',
              'November', 'December']
days_in_month = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
days_of_week = ['Su', 'Mo', 'Tu', 'We', 'Th', 'Fr', 'Sa']

def print_cal(month, year):
    '''
    print_cal(month: int, year: int) --> None

    print_cal is the main functtion that will print the calendar month for
    the specific yaer and month entered.

    print_cal(6, 2000)
    >>>
    Jun 2000
    Su Mo Tu We Th Fr Sa
                1  2  3  
    4  5  6  7  8  9  10 
    11 12 13 14 15 16 17 
    18 19 20 21 22 23 24 
    25 26 27 28 29 30 

    print_cal(1, 1)
    >>>
    Jan 1
    Su Mo Tu We Th Fr Sa
       1  2  3  4  5  6  
    7  8  9  10 11 12 13 
    14 15 16 17 18 19 20 
    21 22 23 24 25 26 27 
    28 29 30 31     
    '''
    
    month_corrected = month_list[month-1]
    days = days_in_month[month-1]
    
    day_one = datetime.date(year, month, 1)
    day_start = day_one.isoweekday()

    print(month_corrected[:3], year)
    print(' '.join(['{0:<2}'.format(w) for w in days_of_week]))

    print('{0:>3}'.format('')*day_start, end='')

    if day_start >= 7:
        print()
        start_day = 0

    
    for i in range (1, days + 1):
        # prints i and then 2 spaces
        print(f'{i:<2}', end = ' ')
        day_start += 1
        # Once we start a new week, start new line
        if day_start >= 7:
            day_start = 0
            print()
    print()
    return

def main():
    '''
    main() is the main function of the program. Runs an infinite loop that
    prompts the user for a month. The infinate loop works well here because
    it will keep asking the user for a month/ year untill a valid input is
    entered

    Calls print_cal()
    
    >>> main()
    Enter A Number Between 1 and 12: 
    '''
    while True:
        month = int(input("Enter A Number Between 1 and 12: "))
        if month > 12 or month < 1:
            print("Please Enter A Valid Number")
        year = int(input("Enter A Year: "))
        if year <= 0:
            print("Please Enter A Valid Year")
        print_cal(month,year)
    return

def test_print_cal():
    '''
    Test print_cal() function with a Basic, Edge and an Error case

    repediatly calls print_cal with test cases

    print_cal(6, 2000)
    >>>
    Jun 2000
    Su Mo Tu We Th Fr Sa
                1  2  3  
    4  5  6  7  8  9  10 
    11 12 13 14 15 16 17 
    18 19 20 21 22 23 24 
    25 26 27 28 29 30 

    print_cal(1, 1)
    >>>
    Jan 1
    Su Mo Tu We Th Fr Sa
       1  2  3  4  5  6  
    7  8  9  10 11 12 13 
    14 15 16 17 18 19 20 
    21 22 23 24 25 26 27 
    28 29 30 31 

    print_cal(0, 1)
    >>> ValueError: month must be in 1..12
    '''
    print_cal(6, 2000) # Basic case
    print_cal(1, 1) # Edge case
    print_cal(0, 1) # Error case
