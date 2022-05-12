'''
Title - Earthquake Report

Author - Ryan Heise

Credit - N/A
'''
from p6a_data_analysis import *

def equake_readf(fname):
    '''
    (fname: str) -> List

    This function reads the file and returns a list of the earthquake magnitudes

    >>> equake_readf('test.txt')
    [1,2,3,4,5,6]
    '''
    magnitudes = []
    with open(fname, 'r') as file:
        header = file.readline().split(',')
        for line in file:
            magnitudes.append(float(line.split(',')[4]))
    return magnitudes

def equake_analysis(magnitudes):
    '''
    (magnitudes: List) -> int

    This funstion takes in the magnitude list from equake_readf() and spits out
    the mean, median, and mode

    >>> equake_analysis([1,2,3)
    [Mean 3, Median 2, Mode 0]
    '''
    global mean, median, mode
    
    mean = mean(magnitudes)
    median = median(magnitudes)
    mode = mode(magnitudes)

    return [("Mean", mean), ("Median", median), ('Mode', mode)]

def equake_report(magnitudes, mmm, minmag):
    '''
    (magnitudes: list, mmm: list, minmag: int) -> None

    Takes in data from previous functions and prints out data
    '''
    minmag_count = 0
    for item in magnitudes:
        if item > minmag:
            minmag_count += 1
    
    print("    Earthquake Data Analysis")
    print("        25 Years Ago to Present")
    print("      250km Centered at Eugene, OR\n")
    print("There have been", minmag_count, "earthquaks with magitude", minmag, "or higher\nover the past 25 years.\n")
    print("Mean magnitude is: ", mmm[0][1])
    print("Median magnitude is: ", mmm[1][1])
    print("Mode magnitude is: ", str(mmm[2][1]))
    print()
    frequencyTable(magnitudes)


def main():
    '''()-> None
    Calls: equake_readf, equake_analysis, equake_report
    Top level function for earthquake data analysis.
    Returns None.
    '''
    fname = 'p7b-equakes25f.csv'
    minmag = 2.5
    #fname = 'p7b-equakes50f.csv'
    #minmag = 5.0

    emags = equake_readf(fname)
    mmm = equake_analysis(emags)
    equake_report(emags, mmm, minmag)
    return

main()
