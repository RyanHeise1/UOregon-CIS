'''
Title: Project 4b testing and debugging

Author: Ryan Heise

Credit: N/A
'''

import doctest

def ratsBug(weight, rate):
    '''(number, number) -> tuple

    Return number of weeks it will
    take for a rat to weigh 1.5 times
    as much as its original weight
    (weight > 0) if it gains at rate (rate > 0).

    >>> ratsBug(10, .1)     # normal
    (16.1, 5)
    >>> ratsBug(1, .5)      # edge - just one week
    (1.5, 1)
    '''
    weeks = 0
    if weight <= 0:
        print('Pls enter the correct weight!')
        return None
    if rate <= 0:
        print('rate has to be positive')
        return None
    
    original_wt = weight
    
    while weight < (1.5 * original_wt):
        weight += weight * rate
        weeks = weeks + 1
        
    weight = round(weight, 1)
    return (weight, weeks)

def countSeqBug(astr):
    '''(str) -> int

    Returns the length of the longest recurring
    sequence in astr

    >>> countSeqBug('abccde')  # normal  	
    2
    >>> countSeqBug('')        # edge - empty string
    0
    '''
    if len(astr) != 0:
        prev_item = astr[0]
        dup_ct = 1
        high_ct = 1
    else:
        high_ct = 0
        dup_ct = 0
        
    for i in range(1, len(astr)):
        if astr[i] == prev_item:
            dup_ct += 1
            high_ct = dup_ct

        else:
            prev_item = astr[i]
			
            if dup_ct > high_ct:
                high_ct = dup_ct
            dup_ct = 1

    return high_ct

def my_average(dataset):
    '''(str) -> float

    Returns average of values in input string values,
    but zeros do not count at all.  Return 0 if there
    is no real data.
    
    >>> my_average('23')    #normal, no zeros
    2.5
    >>> my_average('203')   #normal, a zero
    2.5
    >>> my_average('0000')  #all zeros
    0
    >>> my_average('1')     #single item string
    1.0
    >>> my_average('05050505')  
    5.0
    '''
    count = 0
    total = 0
    for value in dataset:
        if value != '0':
            total += int(value)
            count += 1
            
    if count == 0:
        return 0
    
    avg = total / count
    return avg

print(doctest.testmod())
