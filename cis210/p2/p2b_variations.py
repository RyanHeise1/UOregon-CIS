'''
Title: Project 2-b - Python Variations 

Author: Ryan Heise

Credits: N/A
'''

from math import sqrt

def add_digits(n):
    '''
    Finds the sums the digits of a 3-digit integer number

    Parameters: n a positive integer with 3 digits

    Prints result of numbers added together

    >>> add_digits(123)
        The sum of the digits in 123 is 6
    '''
    original_number = n
    sum = 0
    for i in range(3):
        remainder = n % 10
        sum += remainder
        n = int(n / 10)
    print('The sum of the digits in', original_number, 'is', sum)

        
    

def profit(customers):
    '''
    Returns the net profit per show

    Parameters: customers (number of customers)

    returns net profit

    >>> profit(10)
        25.0
    '''
    ticket_cost = 5
    preformance_cost = 20 + (customers * .5)
    customer_cost = customers * ticket_cost

    return (customer_cost - preformance_cost)


def profit_main():
    '''
    call profit ten times, for 5, 10, …, 50 attendees, and report the results

    >>> Profit for 35 customers would be: $ 137.5
    >>> Profit for 40 customers would be: x
    '''
    
    for i in range(10):    
        customer_ammount = (i + 1) * 5
        net_profit = profit(customer_ammount)
        print('Profit for ', customer_ammount, 'customers would be: $', net_profit)
        

def gravity(v):
    '''
    print the time it will take to reach the ground, rounded to two decimal places

    Parameters: v (velocity)

    >>> gravity(1)
        Time to ground from 11000 meters for 1 m/sec is 47.28 seconds.
    '''
    top_fcn = v - (sqrt(v**(2)-4*(-4.9)*(11000)))
    bottom_fcn = 2*(-4.9)
    time = top_fcn / bottom_fcn
    result = round(time, 2)
    print("Time to ground from 11000 meeters for", v, "m/sec is", result, "seconds")
    return
