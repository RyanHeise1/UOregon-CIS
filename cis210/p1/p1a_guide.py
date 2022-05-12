'''
Title: Project 1-a - UO Guide 

Author: Ryan Heise

Credits: N/A
'''
from turtle import *

def uo_guide():
    '''
    Welcome to the UO!  Welcome to Computer Science!
    Guide students from the EMU Lawn to Deschutes Hall, home of the
    Computer Science Department, and then to Price Science Commons
    (Science Library), home of B004/A computer lab and study space.

    >>> uo_guide()    runs the program
    '''
    # set the scene 
    reset()
    clear()
    title('Welcome to Computer Science at the University of Oregon!')
    color('purple')
    pensize(3)
    speed('slowest')
    bgpic('/Users/ryanheise/Documents/210/uo_campus_map.png') # File path to get to headder pic (UO Campus)
    screensize(1195, 681)   

    stamp() #mark start of route on EMU East lawn
   
    # guide to Deschutes
    fd(5)
    rt(90)  #1st turn
    fd(14)
    lt(90)  #2nd turn
    fd(60)
    lt(90)  #3rd turn
    fd(65)
    lt(90)  #4th turn
    fd(25)
    rt(90)  #5th turn
    fd(15)
    rt(90)  #6th turn
    fd(10)  #DESTINATION ARRIVED
    stamp() # Marks Location at Deschutes
    
    
    # guide to Price Science Commons
    pu()    #Pen Up for turtle to backtrack to 13th ave
    bk(10)
    lt(90)
    bk(15)
    lt(90)
    pd()    # Pen down to resume navigation
    fd(95)
    rt(90)  # 1st Turn
    fd(30)
    rt(45)  # 2nd Turn
    fd(20)  #DISTINATION ARRIVED
    stamp()
    
    return
