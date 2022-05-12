'''
Title - Project 6c Zip To Bar Code

Author - Ryan Heise

Credit - None
'''

from turtle import *

encode = [[1, 1, 0, 0, 0],	# encoding for '0'
         [0, 0, 0, 1, 1],	# encoding for '1'
         [0, 0, 1, 0, 1],       # encoding for '2'
         [0, 0, 1, 1, 0],	# encoding for '3'
         [0, 1, 0, 0, 1],	# encoding for '4'
         [0, 1, 0, 1, 0],	# encoding for '5'
         [0, 1, 1, 0, 0],	# encoding for '6'
         [1, 0, 0, 0, 1],	# encoding for '7'
         [1, 0, 0, 1, 0],	# encoding for '8'
         [1, 0, 1, 0, 0]	# encoding for '9'
        ]

short_length = 25

def jump(x, y):
    '''(x: number, y: number) 

    move turtle without leaving a track to position (x,y).
    like turtle setpos but pen is always up for the move
    and put down after

    >>> jump(0,0)
    [return turtle to center]
    '''
    pu()
    setpos(x,y)
    pd()

    return

def long():
    '''
    () -> None

    This functions main purpoise to draw the first and last line for the turtle
    '''
    lt(90)
    fd(50)
    pu()
    bk(50)
    rt(90)
    fd(10)
    pd()
    return

def CheckDigit(digits):
    '''
    (str) -> int

    This fcn returns the check digit for the final number in the barcode

    >>> CheckDigit('97403')
    3    
    '''
    sum = 0
    new_zip = int(digits)
    while new_zip > 0:
        remainder = new_zip % 10
        sum += remainder
        new_zip = int(new_zip / 10)
    check_digit = 10 - (sum % 10)
    return check_digit

def draw_bar(zipcode):
    '''
    (str) -> None

    This fcn draws the zipcode into a barcode using turtle graphics

    >>> draw_bar(zipcode)
    (Draws board)
    '''
    lt(90)
    if zipcode == 0:
        length = short_length
    else:
        length = 2 * short_length
    fd(length)
    pu()
    bk(length)
    rt(90)
    fd(10)
    pd()
    return

def zipToBar(zipcode):
    '''
    (str) -> None

    Fucntion that draws the barcode using turtle graphics

    Calls CheckDigit to find the last digit to the barcode
    Calls encode to figure out the bianary sequence
    Calls draw_bar to draw the barcode

    returns none
    '''
    check_digit = CheckDigit(zipcode)
    jump(-200,0)
    long()
    # Draw the first 5 digits
    for a in str(zipcode):
        digit = int(a)
        for b in encode[digit]:
            draw_bar(b)
    # Draw the last digit (the check digit)
    for c in str(check_digit):
        last_digit = int(c)
        for d in encode[last_digit]:
            draw_bar(d)
    long()
    
    
            

def main():
    '''
    main fcn that begins the program

    >>> main()
    draws barcode for "97403"
    '''
    zipToBar('97403')
    
 



