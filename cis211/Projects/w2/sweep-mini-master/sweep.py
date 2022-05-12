"""
Project desc - This is the mini project for week 2, There are 3 fuctions in this file

    all_same spits out a bool determing if all the numbers in a file are true
    max_run spits out the number which occured the most in the list
    dedup returns a de-duplicated version of the input list (ie: a list without duplicate values)

Name - Ryan Heise
Due - 1/15/21
Credit - None
"""

def all_same(l: list) -> bool:
    """
    Purpose: This function returns true if all the items in a list are the same

    Examples:
        all_same([1, 1, 1, 1]) will return True
        all_same([]) will return True
        all_same([1, 3, 1, 1]) will return False
    """

    if len(l) == 0:
        return True
    for item in l:
        if item != l[0]:
            return False
    return True



def max_run(l: list) -> int:
    """
    This function takes in a list and returns the number of occurences the most relevant number has

    :param l: a list of int's
    :return: int

    Examples -
    max_run([1, 1, 1, 2, 2, 3, 1, 1])
    3 (because 1 occurs 3 times in a row)

    How to solve this -
        the value that all the items in the current run match
        the length of the current run, so far
        the length of the longest run I have found so far


    """
    if len(l) == 0:
        return 0

    current_run = l[0]
    len_run = 0
    long_run = 0

    for item in l:
        if item == current_run:
            len_run += 1
            if long_run < len_run:
                long_run += 1
        else:
            current_run = item
            len_run = 1
    return long_run


def dedup(l: list) -> list:
    """
    I keep track of the value in the current run while building up a result

    :param l: list
    :return: a list
    """
    result = []
    if len(l) == 0:
        return result
    current_run = l[0]
    result.append(current_run)
    for item in l:
        if item == current_run:
            pass
        else:
            result.append(item)
            current_run = item
    return result