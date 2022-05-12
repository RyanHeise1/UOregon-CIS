'''
Title: Project 2-b - Art Show


Credits: N/A
'''
from turtle import *

def jump(x, y):
    '''
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

    # after each function is written, uncomment the line
    # that calls the function
    house()
    flower()
    sunshine()

    return

def square():

    pencolor('black')
    pensize(1)
    fillcolor('blue')

    begin_fill()
    fd(100)
    rt(90)
    fd(100)
    rt(90)
    fd(100)
    rt(90)
    fd(100)
    end_fill()

def triangle():
    pensize(1)
    pencolor('black')
    fillcolor('red')
    begin_fill()
    rt(90)
    fd(100)
    lt(120)
    fd(100)
    lt(120)
    fd(100)
    end_fill()
    jump(0,0)

    return



def flower():
    rt(150)
    jump(-200,-100)
    pensize(4)
    pencolor('green')
    fd(200)
    for i in range(36):
        jump(-200,100)
        triangle()
        lt(10)

def house():
    square()
    triangle()
    return

def sunshine():
    jump(250,250)
    for i in range (36):
        pensize(5)
        pencolor('yellow')
        fd(100)
        jump(250,250)
        rt(10)
