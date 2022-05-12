'''
CIS 122 Winter 2020 Assignment 3
Author: Ryan Heise
Partner: Jayden Ben
Description: Create a void function to draw a line using Python's Turtle graphics
'''

import turtle
t = turtle.Turtle()

def draw_line(t, x, y, angle, length):
   """This function will draw the line of the x, y, angle, and length you enter

   The function starts with the pen up. So the turtle can go to the x, y, and angle
   then, the pen will go down and draw the line according to the length you enter.

Args:
   t (Turtle): Drawing Turtle
   x (int/float): starting x location
   y (int/float): Starting y location
   angle: Angle tutle starts at
   length: length of the line

   Returns:
      None
"""

   t.pu()
   t.setx(x)
   t.sety(y)
   t.seth(angle)
   t.pd()
   t.fd(length)
   t.pu()

# draw_line(t, 100, 100, 0, 200)
# draw_line(t, -100, -100, 270, 50)
# draw_line(t, 200, -200, 45, 75)


def draw_radial_lines(t, x, y, length, num_lines):
   """this function will draw the radial lines using turtle, x and y cords, lenght of the line, and number of lines

    the function starts with the angle being 0, it will repeat 16 times first time 0 then 22.5 and so on. after it calcs
    the angle it will draw the line using the previous function.

Args:
   t (Turtle): Drawing Turtle
   x (int/float): starting x location
   y (int/float): Starting y location
   length: length of the line
   num_lines: specifys the number of lines do draw

   Returns:
      None
"""

   angle = 0
   for line in range(16):
      draw_line(t, x, y, angle, length)
      angle = angle + 22.5


def draw_radials_in_quadrants(t, length, num_lines):
   """This is the final function that will draw the radial in a specific quadrent specified below

   The function wants specifys the quadrent you will be drawing in: -100, -100 for quadrent III

Args:
   t (Turtle): Drawing Turtle
   length: length of the line
   num_lines: specifys the number of lines to draw

   Returns:
      Lines in the specified quadrent 
"""

   draw_radial_lines(t, -100, -100, length, num_lines)
   draw_radial_lines(t, 100, -100, length, num_lines)
   draw_radial_lines(t, -100, 100, length, num_lines)
   draw_radial_lines(t, 100, 100, length, num_lines)



draw_radials_in_quadrants(t, 75, 8)
draw_radials_in_quadrants(t, 50, 16)