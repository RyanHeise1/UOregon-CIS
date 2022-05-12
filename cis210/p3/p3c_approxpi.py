'''
Title: Project 3-c - Monte Pi (Approx Pi)

Author: Ryan Heise

Credits: N/A
'''

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

def main():
    '''
    Runs the reportPi() function with 1000 interations
    '''
    k = 1000
    mypi = montePi(k)
    reportPi(k, mypi)
    return
