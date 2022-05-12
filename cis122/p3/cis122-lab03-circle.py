'''
CIS 122 Winter 2020 Lab 3
Author: Ryan Heise
Partner: Jayden Ben
Description: Create a function draw_concentric_circles() that will draw n number of circles, with an initial size (radius),
a specific gap between each circle, and a starting x and y location
'''

import turtle
t = turtle.Turtle()

def move(t, x, y):
   """
   Move Turtle to x, y position
   """
   t.pu()
   t.goto(x, y)
   t.pd()

def draw_circle(t, radius, x, y):
   """
   Draw a circle of length radius using Turtle t at position x, y
   """
   move(t, x, y - radius)
   t.circle(radius)
   

def draw_concentric_circles( num ,radius ,line_gap ,x ,y ):
   """
   Draw Circle using a for loop with arguments of number of circles, radius, line gap, and x/y cords
   """
   for i in range(num):
      draw_circle(t, radius ,x ,y)
      radius = radius + line_gap

draw_concentric_circles(3, 50, 25, 0, 0)
