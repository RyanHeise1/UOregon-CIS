'''
Title: Project 3-1 - Create Your Own


Credits: Used the fucn sunshine that i used in a previous assignment


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

def sunshine():
    '''
    Draws the sun

    Runs when you call >>> create_main()
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

def body():
    '''
    Drawn the stick figures body

    runs when you call create_main()
    '''
    rt(90)
    forward(100)
    pu()
    bk(100)
    lt(90)
    pd()
    return

def legs():
    '''
    Draws 2 legs for the stick figure

    runs when you call create_main()
    '''
    rt(90)
    fd(100)
    rt(45)
    pd()
    fd(30)
    back(30)
    left(90)
    forward(30)
    back(30)
    right(45)
    up()
    back(100)
    left(90)
    return

def arms():
    '''
    Draws both arms of the stick figure

    runs when you call create_main()
    '''
    right(90)
    forward(30)
    left(90)
    down()
    forward(30)
    back(30)
    left(180)
    forward(30)
    back(30)
    left(90)
    up()
    back(30)
    left(90)
    return

def head():
    '''
    Draws a circle for the head

    runs when you call create_main()
    '''
    down()
    circle(25)
    up()
    return

def figure():
    '''
    call all the previous functions to draw the stick figure
    '''
    body()
    legs()
    arms()
    head()
    return


def origami():
    '''
    Draws figure 5 times

    runs when you call create_main()
    '''
    for i in range (4):
        figure()
        fd(100)
        figure()
    return

def create_main():
    '''
    Main function that runs the program

    calls for the r=oragami figure to be drawn along with the sun

    Run function using >>> create_main()
    '''
    reset()
    clear()
    ht()
    title('I Miss Parties :(')
    speed('fastest')
    jump(-300,0)
    origami()
    sunshine()

    return
