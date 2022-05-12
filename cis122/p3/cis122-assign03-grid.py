'''
CIS 122 Winter 2020 Assignment 3
Author: Ryan Heise
Partner: None
Description: Write a void function draw_grid() that accepts a single integer, and will print a grid of numbers
'''


def draw_grid(num):
    for x in range(1, num + 1):
        for y in range(1, num + 1):
            print(y, end = ' ')
        print()

draw_grid(4)