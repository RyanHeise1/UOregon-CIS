'''
Title - p7c File Counts

Author - Ryan Heise

Credit - N/A
'''
from p6a_data_analysis import *

def fcounts(fname):
    '''
    (fname: str) -> None

    This function takes in a file and spits out data about it

    >>> fcounts('tinyf.txt')
    in file tinyf.txt:
    The number of lines is: 2
    The number of words is: 3
    The number of characters (no spaces) is: 18
    The number of characters (with spaces) is: 19
    Word occurrences in file p7c-3.txt:
    ITEM FREQUENCY
    hello 2
    world 1 
    '''
    word_ct = 0
    lines_ct = 0
    noSp = 0
    wordList = []

    # Line Counter
    with open(fname) as lineCount:
        readLine = lineCount.read()
        splitLine = readLine.split("\n")
        for lines in splitLine:
            if lines:
                lines_ct += 1

    # Word Counter
    with open(fname) as wordCount:
        for line in wordCount:
            words = line.split()
            word_ct += len(words)
            
    # Character Counter (No Space)
    with open(fname) as chrCount:
        for line in chrCount:
            noSp += len(line.split('\n')[0])
        num_charsx = noSp - line.count(' ')
        
    # Character Counter (With Space)
    with open(fname) as chrCountSpace:
        chrSpace = chrCountSpace.read()
        space = len(chrSpace)

    # Frequency Table
    with open(fname) as table:
        for line in table:
            wordList.extend(line.split())
        
    
        
            

    print("In file " + fname + ":")
    print("The number of lines is: ", lines_ct)
    print("The number of words is: ", word_ct)
    print("The number of characters (no spaces) is: ", noSp)
    print("The number of characters (with spaces) is: ", space)
    print()
    print("Word occurrences in file" + fname + ':')
    frequencyTable(wordList)    


def main():
    '''
    () -> None

    Main function that runs fcount()

    >>> main()
    in file tinyf.txt:
    The number of lines is: 2
    The number of words is: 3
    The number of characters (no spaces) is: 18
    The number of characters (with spaces) is: 19
    Word occurrences in file p7c-3.txt:
    ITEM FREQUENCY
    hello 2
    world 1 
    '''
    fcounts('tinyf.txt')
