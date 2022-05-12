'''
Title: Project 2-b - Art Show 

Author: Ryan Heise

Credits: N/A
'''
from turtle import *

def jump(x, y):
    '''
    move turtle without leaving a track to position (x,y).
    like turtle setpos but pen is always up for the move
    and put down after

    >>> jump(0,0) to jumo to center of screen
    [return turtle to center]
    '''
    pu()
    setpos(x,y)
    pd()

    return

def art_show_main():
    '''
    draw a picture of a house and sun and flower
    
    calls: house, sunshine, flower

    >>> art_show_main()
    '''
    reset()
    clear() 
    title("A Sunny Day")
    speed('fastest')
    hideturtle()
    house()
    flower()
    sunshine()
    

def house():
    '''
    Draws a house

    Runs when you call >>>art_show_main()
    '''
    # Draw base of house
    fillcolor('blue') # Sets fill color to fill base of the house blue
    jump(-100,0)
    begin_fill() # Begins to draw house
    fd(200)
    lt(90)
    fd(100)
    lt(90)
    fd(200)
    lt(90)
    fd(100)
    end_fill() # Finish Drawing House

    # Draw Roof of house
    jump(0,150)
    fillcolor('black')
    begin_fill()
    lt(63.435)
    fd(111.803)
    rt(153.435)
    fd(200)
    rt(153.435)
    fd(111.803)
    end_fill()

    
def flower():
    '''
    Draws a flower
    
    Runs when you call >>>art_show_main()
    '''
    lt(153.435)
    
    # Draw stem of flower with increased width
    jump(-250,0)
    rt(90)
    pensize(5)
    pencolor('green')
    fd(200)

    # Change Pen color/width
    pencolor('black')
    pensize(1)
    
    # For loop to draw triangles as leaves
    for i in range (10):
        fillcolor('red')
        begin_fill()
        rt(90)
        fd(80)
        lt(120)
        fd(80)
        lt(120)
        fd(80)
        jump(-250,200)
        end_fill()
        rt(14)
        
        
    
def sunshine():
    '''
    Draws the sun
    
    >Runs when you call >>> rt_show_main()
    '''
    # Draw Sun using for loop
    jump(250,250)
    for i in range (36):
        pensize(5)
        pencolor('yellow')
        fd(100)
        jump(250,250)
        rt(10)

    return 
