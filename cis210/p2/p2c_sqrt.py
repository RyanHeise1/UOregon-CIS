'''
Title: Project 2-c - Sqrt function 

Author: Ryan Heise

Credits: N/A
'''

from math import sqrt

def mysqrt(n, k):
    '''
    The function uses the equation given to determine the approximate square root of n

    Parameters:
        n: Positive integer
        k: number of times the process should run

    Returns: result
    '''
    count = 0
    x_at_zero = 1

    while(count < k):
        x_sub_k = (x_at_zero + (n / x_at_zero)) / 2
        x_at_zero = x_sub_k
        count += 1
    return (x_at_zero)

def sqrt_compare(num, iterations):
    '''
    Function that compares math.sqrt vs the Babylonian method

    Parameters:
        num: a positive integer to find the square root of
        iterations: the number of times the iterative square root approximation process should run

    Returns None
    Prints result comparing them
    '''
    print("For " + str(num) + " using " + str(iterations) + " iterations:")
    math_sqrt_fcn = sqrt(num)
    my_sqrt_fcn = mysqrt(num,iterations)
    print("mysqrt value is: "+str(my_sqrt_fcn))
    print("math lib sqrt value is: "+str(math_sqrt_fcn))
    percent_error = round(abs(math_sqrt_fcn - my_sqrt_fcn),2)
    print("The difference is " + str(percent_error))

    return

def sqrt_compare_mail():
    '''Square root comparison program driver.'''
    sqrt_compare(25, 5)
    sqrt_compare(25, 10)
    sqrt_compare(625, 5)
    sqrt_compare(625, 10)
    sqrt_compare(10000, 5)
    sqrt_compare(10000, 10)
    

