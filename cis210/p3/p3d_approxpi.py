'''
Title: Project 3-c - Monte Pi (Approx Pi)

Author: Ryan Heise

Credits: N/A
'''

from turtle import *
import random
import math

def montePi(numDarts):
    '''
    approximates value for π that is generated.

    parameter:
        numDarts:  number of times the approximating π process should run

    >>> montePi(100)
    3.08   
    >>> montePi(100000)
    3.143072
    >>> montePi(10000000)
    3.1418752
    '''
    
    inCircle = 0
    
    for i in range(numDarts):
        x = random.random()
        y = random.random()

        distance = math.sqrt(x**2 + y**2)

        if distance <= 1:
            inCircle += 1

    pi = inCircle / numDarts * 4

    return pi

def isInCircle(x, y, r):
    '''
    Tests to see if point given will be in circle or not

    Parameters:
        x: x value
        y: y value
        r: radius of circle

    Returns True if the input point is inside the circle centered at
    point (0,0) with radius r, and False otherwise.

    Called by montePi()
    
    >>> isInCircle(0, 0, 1)
    True
    >>> isInCircle(.5, .5, 1)
    True
    >>> isInCircle(1, 2, 1)
    False
    '''
    r = 1
    distance = math.sqrt(x**2 + y**2)
    if distance <= r:
        return True
    else:
        return False

def drawBoard():
    '''
    no param -> turtle graphics

    Sets up turtle graphics x and y axis

    '''
    wn = Screen()
    wn.setworldcoordinates(-2, -2, 2, 2)

    speed('fastest'); hideturtle()
    penup()

    goto(-1, 0)
    pendown()
    goto(1, 0)
    penup()
    goto(0, 1)
    pendown()
    goto(0, -1)
    penup()
    goto(0, -1)
    return None

def reportPi(numDarts, approxPi):
    '''
    Computes the difference between the approximate value for π generated
    by the Monte Carlo simulation and the value of pi in the Python math module

    Parameters:
        numDarts: number of times the approximating π process should run
        approxPi:  approximate value of π generated in montePi

    Prints the percentage error

    >>> reportPi(1000, mypi)
    With 1000 iterations:
    my approximate value for pi is: 3.104
    math lib pi value is: 3.141592653589793
    This is a 1.2 percent error.
    '''

    percent = abs(((approxPi - math.pi) / math.pi) * 100)
    
    print("With " + str(numDarts) + " iterations:")
    print("My approximate value for pi is:", approxPi)
    print("Math lib pi value is: " + str(math.pi))
    print("This is a", round(percent, 2), "percent error")

def showMontePi(numDarts):
    '''
    (int) -> turtle

    Calls on isInCircle to draw, in turtle graphics, blue points for points that are within the circle,
    and red points for those outside the circle

    >>>showMontePi(100)
    >>>With 100 iterations:
       my approximate value for pi is: 3.12
       math lib pi value is: 3.141592653589793
       This is a 2.4 percent error
       3.12
    '''
    # set up canvas and turtle
    # to animate the algorithm;
    # draw x, y axes
    # new drawBoard function will replace
    drawBoard()
    # pen should stay up for drawing darts
     
    inCircleCt = 0
    r = 1
    # throw the darts and check whether
    # they landed on the dart board and
    # keep count of those that do   
    for i in range(numDarts):
        x = random.random()     #the random values of x and y generated here will produce different points each time it is run
        y = random.random()     # hence results will vary by every example    
        if isInCircle(x,y,r) == True:       #calling function isInCircle
            inCircleCt += 1
            color('blue')                   #blue for True values
        else:
            color('red')                    #red for False values-outside of Circle
            
    # Revise code to call new isInCircle function.
    # See Project 3-2 and 3-3 specifications.


        # show the dart on the board
        goto(x, y)
        dot()

    # calculate approximate pi
    approxPi = inCircleCt/numDarts * 4
    reportPi(numDarts,approxPi)
    # ADD CODE HERE to compare approxPi to math.pi
    # call reportPi function to do this.
    # See Project 3-3 specification

    #wn.exitonclick()

    return approxPi

def main():
    print(showMontePi(1000))

main()
