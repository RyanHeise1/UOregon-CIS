'''
Title: Project 2-a - Mars rover


Credits: Starter code given in the Mars Rover assignment
'''
from turtle import *
from random import randint # Fixed from started code


def rover_loc():
    '''
    Randomises cords so the mars rover can move and collect data on the screen.
    Returns a random int between -275 and 275

    >>> rover_loc()
        238 [for example]

    called twice by mars_explore() to find x and y cords
    '''
    return randint(-275,275)

def water_content():
    '''
    Randomises a number between 1 and 290 to simulate finding water content on mars

    Returns random number between 1 and 290 (ppm)

    >>> water_content()
        56 [for example]

    called by mars_explore()
    '''
    return randint(1,290)

def temperature():
    '''
    Randomises a number between -178 and 1 to simulate finding the temp on mars

    Returns number between -178 and 1

    >>> temperature()
        -194 [for example]

    called by mars_explore()
    '''
    return randint(-178,1)

def mars_explore():
    '''
    Calls rover_loc twice to get x and y pos

    Moves turtle to the values and uses "stamp" function to show path
    '''
    display_color = 'blue'
    dot(10, display_color)

    xPos= rover_loc()
    yPos= rover_loc()

    WaterContent= water_content()
    temp= temperature()

    goto(xPos,yPos)

    write(WaterContent, font= 8, align='left')
    write(temp, font= 8, align='right')

    dot(10, display_color)

    print(xPos, '\t', yPos, '\t', WaterContent, '\t', temp)




def mars_explore_main():
    '''
    main function for mars_explore:
        set up print and graphical output
        then call mars_explore repeatedly

    >>> mars_explore_main()
    '''
    print('xpos', '\t',
          'ypos', '\t',
          'water', '\t',
          'temp')               # label for print output - use tab

    # set up graphical output
    reset()
    title('Mars Rover')
    display_color = 'blue'
    pencolor(display_color)
    dot(10, display_color)      # mark the rover's starting position
    #shape("turtle")             # Changes Turtle Shape To Look Like A Turtle :)

    # explore Mars

    for i in range(20):
        mars_explore()

    return
