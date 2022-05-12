'''
Title - Project 9a Bianary Search

Author - Ryan Heise

Credit - N/A
'''

def isMemberI(aseq, target):
    '''
    (aseq: sequence, target: int) -> bool

    This function will take in a sequence with a target and will return
    a boolean if the tarter is an element or not

    >>> isMemberI((1,3,4,6,8,9,11), 4)
    True
    
    >>> isMemberI((),1)
    False
    '''
    start = 0
    end = len(aseq) - 1
    while start <= end:
        mid = (start + end) // 2
        if aseq[mid] == target:
            return True
        elif target < aseq[mid]:
            end = mid - 1
        else:
            start = mid + 1

    return False

def isMemberR(aseq, target):
    '''
    (aseq: sequence, target: int) -> bool

    function isMemberR uses a recursive approach to implement the bianary search

    >>> isMemberR((99, 100), 101)
    False 

    >>> isMemberI((99, 101), 101)
    True
    '''
    if len(aseq) == 0:
        return False
    else:
        mid = len(aseq) // 2

        if aseq[mid] == target:
            return True
        elif aseq[mid] > target:
            return(isMemberR(aseq[:mid], target))
        else:
            return(isMemberR(aseq[mid+1:], target))


def bintest(f):
    '''
    (f: fucntion) -> None

    Function bintest is used to test the bianarys earch functions
    '''
    print(f((1, 2, 3, 3, 4), 3))        # True
    print(f((1, 2, 3, 3, 4), 99))       # False
    print(f(('aeiou'), 'i'))            # True
    print(f(('aeiou'), 'y'))            # False
    print(f((1, 3, 5, 7), 4))           # False
    print(f((23, 24, 25, 26, 27), 5))   # False
    print(f((0, 1, 4, 5, 6, 8), 4))     # True
    print(f((0, 1, 2, 3, 4, 5, 6), 3))  # True
    print(f((1, 3), 1))                 # True
    print(f((2, 10), 10 ))              # True
    print(f((99, 100), 101))            # False
    print(f((42,), 42))                 # True
    print(f((43,), 44))                 # False
    print(f((), 99))                    # False

def main():
    ''' () -> None
    Main Aux funnction that calls isMemberR and isMemberI to test them using
    bintest(f)
    '''
    bintest(isMemberR)
    print()
    bintest(isMemberI)
    print()
    return None

main()
