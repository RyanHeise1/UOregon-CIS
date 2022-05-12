'''
Title - project 7 majors

Author - Ryan Heise

Credit - N/A
'''

def majors_readf(fname):
    '''
    (fname: str) -> List

    This function returns a list of the majors in the file

    >>> majors_readf('majors-short.txt')
    ['CIS', 'SDSC', 'CIS', 'EXPL', 'PBA', 'CIS', 'CIS', 'PHYS', 'DSCI']
    '''
    with open(fname) as majors:
        majors.readline()
        majors.readline()
        majorli = []
        majorli = majors.readlines()

        for line in range(len(majorli)):
            majorli[line] = majorli[line].strip()
    return majorli
            

def majors_analysis(majorsli):
    '''
    (majorsli: List) -> List

    This function takes in a list of majors and determines the most
    frequent occuring majors

    >>> majors_analysis(['CIS', 'SDSC', 'CIS', 'EXPL', 'PBA',
                         'CIS', 'CIS', 'PHYS', 'DSCI'])
    (['CIS'], 6)
    '''
    num = 0
    majors_ct = len(set(majorsli))
    majors_mode = majorsli[0]
    
    # finds the mode
    for items in majorsli:
        curr_freq = majorsli.count(items)
        if curr_freq > num:
            num = curr_freq
            majors_mode = items
               
    return majors_mode, majors_ct

def majors_report(majors_mode, majors_ct, majorsli):
    '''
    (majors_mode: str, majors_ct: int, majorsli: List) -> None

    This function prints out the most frequent item in a given .txt file,
    the ammount of majors, and the item frequency of that list.

    >>> majors_report('CIS', 6, ['CIS', 'SDSC', 'CIS', 'EXPL',
                         'PBA','CIS', 'CIS', 'PHYS', 'DSCI'])
    6 majors are represented in CIS 210 this term.
    The most represented major(s): CIS
    ITEM FREQUENCY
    CIS 4
    DCSI 1
    EXPL 1
    PBA 1
    PHYS 1
    SDSC 1
    '''
    # for loop thhat creates an array of the majors that arent dups
    seen = set()
    unique = []
    for i in majorsli:
        if i not in seen:
            unique.append(i)
            seen.add(i)

    print(majors_ct, "majors are represented in CIS 210 this term.")
    print("The mot represented major(s): ", majors_mode)
    print("Item\tFrequency:")
    for item in unique:
        print(item, '\t', majorsli.count(item))  
    return

def main():
    '''()-> None
    Calls: majors_readf, majors_analysis, majors_report
    Top level function for analysis of CIS 210 majors data.
    '''

    #fname = 'p7a-majors-short.txt'
    fname = 'p7a-majors.txt'

    majorsli = majors_readf(fname) #read
    majors_mode, majors_ct = majors_analysis(majorsli) #analyze
    majors_report(majors_mode, majors_ct, majorsli) #report
    return

if '__name__' == '__main__':
    main()
