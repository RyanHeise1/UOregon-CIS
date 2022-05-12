'''
Title: Project 5a - paritybit

Author: Ryan Heise

Credit: N/A
'''

import doctest

def encode(letter):
    '''
    (str) -> str
    
    encode will add a parity bit to the binary
    (ASCII) representation of letter AKA the leftmost
    number

    >>> encode('c') 
    '01100011'
    >>> encode('d')
    '11100100'
    '''
    bianary = bin(ord(letter))
    new_bin = (bianary[2:])
    fin = parity(new_bin) + new_bin
    return fin

def parity(bitrep):
    '''
    (str)-> str

    Determines the leftmost number or parity given the bianary sequence

    >>> parity('1100011') 
    '0'
    >>> parity('1100100') 
    '1'
    '''
    # initilize variables
    parity = ''
    sum = 0
    new_bitrep = int(bitrep)
    # while loop to count the bianary digit    
    while new_bitrep > 0:
        remainder = new_bitrep % 10
        sum += remainder
        new_bitrep = int(new_bitrep / 10)        
    
    # if sum is even, parity = 0
    if sum % 2 == 0:
        parity = '0'
        return parity
    # if sum is odd, parity = 1
    else:
        parity = '1'
        return parity

def decode(pletter):
    '''
    (str)-> str

    Decode should call function parity, to check for even
    parity in pletter. If no error is detected, decode should
    convert pletter back to a character, and return the
    character. If an error is detected, decode should return
    '*'.

    >>> decode("11110011")
    'c'
    >>> decode('9100')
    '*'
    '''
    left_num = pletter[:1]
    bianary = pletter[1:]
    left_parity = parity(bianary)
    if left_parity != left_num:
        return('*')
    else:
        return chr(int('1100011', 2))

def main():
    letter = 'c'
    print(f'parity bit for {letter} is encode(letter)[0]')


