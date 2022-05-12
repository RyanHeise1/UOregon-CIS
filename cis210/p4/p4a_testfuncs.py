'''
Title: test functions 4a

Author: Ryan Heise

Credit: N/A

Find all substrings of a string, and see if the value
is the same as expacted_value
'''

def sscount0 (needle, haystack):
    '''(str, str) -> int

    Given a "needle" string to search for in a "haystack" string,
    return the count of the number occurrences of the needle in
    the haystack.  Overlapping substrings are included.
    Uses string slice operation (only).
    
    >>> sscount0('sses', 'assesses')
    2
    >>> sscount0('an', 'trans-Panamanian banana')
    6
    >>> sscount0('needle', 'haystack')
    0
    >>> sscount0('!!!', '!!!!!')
    3
    >>> sscount0('o', 'pneumonoultramicroscopicsilicovolcanoconiosis')
    9
    '''
    count = 0
    n = len(needle)
    for i in range(len(haystack)):
        if haystack[i:i+n] == needle:
            count += 1
    return count

def sscount1 (needle, haystack):
    '''(str, str) -> int

    Given a "needle" string to search for in a "haystack" string,
    return the count of the number occurrences of the needle in
    the haystack.  Overlapping substrings are included.
    Using string startswith method simplifies code a bit.
    
    >>> sscount1('sses', 'assesses')
    2
    >>> sscount1('an', 'trans-Panamanian banana')
    6
    >>> sscount1('needle', 'haystack')
    0
    >>> sscount1('!!!', '!!!!!')
    3
    >>> sscount1('o', 'pneumonoultramicroscopicsilicovolcanoconiosis')
    9
    '''
    count = 0
    for i in range(len(haystack)):
        if haystack[i:].startswith(needle):
            count += 1
    return count

def test_sscount(f,args,expacted_result):
    
    '''
    (funtion,string,int)   ->  int

    access sscount0 and sscount1

    testing sscount0
    Checking sses assesses …
    its value 2 is correct! 

    '''
        
    needle = ''

    haystack = ''
    
    needle = args [ 0 : args.index('') ]
    haystack = args [ args.index('') : len(args) ]

    
    
    print('testing ' + f.__name__ )
    print('checking ' + args )
    

    
    if str(f(needle,haystack) == expacted_result):

        print('its value '+str(expacted_result)+' is correct')
    else:
        print('its value '+str(expacted_result)+' is wrong')


def main():
    test_sscount(sscount0, 'sses assesses', 2)
    test_sscount(sscount0,'sses assesses', 2)
    test_sscount(sscount0,'an trans-Panamanian banana', 6)
    test_sscount(sscount0,'needle haystack', 0)
    test_sscount(sscount0,'!!! !!!!!', 3)
    test_sscount(sscount0,'o pneumonoultramicroscopicsilicovolcanoconiosis', 9)
    test_sscount(sscount0,'', 0)
    test_sscount(sscount0,'a ', 0)
    test_sscount(sscount0,' abc', 0)
    test_sscount(sscount0,'a a', 1)

    print('\n')

    test_sscount(sscount1, 'sses assesses', 2)
    test_sscount(sscount1,'sses assesses', 2)
    test_sscount(sscount1,'an trans-Panamanian banana', 6)
    test_sscount(sscount1,'needle haystack', 0)
    test_sscount(sscount1,'!!! !!!!!', 3)
    test_sscount(sscount1,'o pneumonoultramicroscopicsilicovolcanoconiosis', 9)
    test_sscount(sscount1,'', 0)
    test_sscount(sscount1,'a ', 0)
    test_sscount(sscount1,' abc', 0)
    test_sscount(sscount1,'a a', 1)
