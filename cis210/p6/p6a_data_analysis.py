'''
Title - p6a Data Analysis

Author - Ryan Heise

Credit - Textbook Ch 4 for functions mean() median() mode() and frequencytable()
'''


def isEven(n):
    '''
    (n: int -> Bool)

    returns true or false if the number is even or not
    
    >>> isEven(2)
    True
    >>> isEven(5)
    False
    '''
    if n % 2 == 0:
        return True
    else:
        return False


def mean(alist):
    '''
    (alist: list -> float)

    Returns the average of the numbers
        
    >>> mean([1,2,3,4,5])
    3.0
    >>> mean([10,10,10,10])
    10.0
    '''
    mean = sum(alist) / len(alist)
    return mean


def median(alist):
    '''
    (alist: list -> num)

    Locates the item in the middle of the list
    
    >>> median([1,2,3,4,5])
    3
    >>> median([4,2,5,1,3])
    3
    >>> median([10,20,70,80])
    45.0
    '''
    copylist = alist[:]
    copylist.sort()
    median = 0
    if isEven(len(copylist)):
        rightmid = len(copylist) // 2
        leftmid = rightmid - 1
        median = (copylist[leftmid] + copylist[rightmid]) / 2
    else:
        mid = len(copylist) // 2
        median = copylist[mid]
    return median


def mode(alist):
    '''
    (alist: list -> list)

    returns the most frequest item in the list
    
    >>> mode([1,2,3,3,3,4,5])
    [3]
    >>> mode([1,1,1,2,3,3,3,4,5])
    [1, 3]
    '''
    countdict = genFrequencyTable(alist)
    countlist = countdict.values()
    maxcount = max(countlist)
    modelist = []
    for item in countdict:
        if countdict[item] == maxcount:
            modelist.append(item)
    return modelist


def frequencyTable(alist):
    '''
    (alist: list -> none)

    this function calls genFrequencyTable to create the table used to plot the information
        
    > frequencyTable([1, 3, 3, 2])
    ITEM FREQUENCY
    1 1
    2 1
    3 2
    '''
    countdict = genFrequencyTable(alist)
    itemlist = list(countdict.keys())
    itemlist.sort()
    print("ITEM", "FREQUENCY")
    for item in itemlist:
        print(item, " ", countdict[item])



def genFrequencyTable(alist):
    '''
    (alist: list -> none)

    method to find and return the frequency table

    returns countdict

    >>> genFrequencyTable([1,2,3,4,5,6,7,8,9])
    {1: 1, 2: 1, 3: 1, 4: 1, 5: 1, 6: 1, 7: 1, 8: 1, 9: 1}
    '''
    countdict = {}
    for item in alist:
        if item in countdict:
            countdict[item] = countdict[item] + 1
        else:
            countdict[item] = 1
    return countdict


def main():
    '''
    main method to test frequency, mean, median and mode of earthquakes
    '''
    magnitude_earthquakes = [2.9, 3.3, 2.7, 2.8, 2.9, 2.6, 5.3, 6.0, 3.0, 5.3, 2.7, 4.3, 5.4, 4.4, 2.6, 2.8, 4.4, 4.3,
                             4.7, 3.3, 4.0, 2.5, 4.9, 4.9, 2.5, 4.8, 3.1, 4.9, 4.4, 6.6, 3.3, 2.5, 5.0, 4.8, 2.5, 4.2,
                             4.5, 2.6, 4.0, 3.3, 3.1, 2.6, 2.7, 2.9, 2.7, 2.9, 3.3, 2.8, 3.1, 2.5, 4.3, 3.2, 4.6, 2.8,
                             4.8, 5.1, 2.7, 2.6, 3.1, 2.9, 4.2, 4.8, 2.5, 4.5, 4.5, 2.8, 4.7, 4.6, 4.6, 5.1, 4.2, 2.8,
                             2.5, 4.5, 4.6, 2.6, 5.0, 2.8, 2.9, 2.7, 3.1, 2.6, 2.5, 3.2, 3.2, 5.2, 2.8, 3.2, 2.6, 5.3,
                             5.5, 2.7, 5.2, 6.4, 4.2, 3.1, 2.8, 4.5, 2.9, 3.1, 4.3, 4.9, 5.2, 2.6, 6.7, 2.7, 4.9, 3.0,
                             4.9, 4.7, 2.6, 4.6, 2.5, 3.2, 2.7, 6.2, 4.0, 4.6, 4.9, 2.5, 5.1, 3.3, 2.5, 4.7, 2.5, 4.1,
                             3.1, 4.6, 2.8, 3.1, 6.3]
    frequencyTable(magnitude_earthquakes)
    print('Mean: ', mean(magnitude_earthquakes))
    print('Median: ', median(magnitude_earthquakes))
    print('Mode: ', mode(magnitude_earthquakes))


# Runs the doctest
if __name__ == "__main__":
    import doctest
    doctest.testmod()

#running main method
main()
