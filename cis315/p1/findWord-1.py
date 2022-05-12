"""
How to run:
    python3 findWord.py < inputFile.txt

Note:
    inputFile must have the current number of words in the file

Sample InputFile:
3
theyouthevent
theeaglehaslande
lukelucklikeslakeslukeducklikeslakeslukelucklickslakesluckducklickslakes

Sample Output:
phrase number: 1
theyouthevent

iterative attempt:
YES, can be split
the you the vent

memoized attempt:
YES, can be split
the you the vent


phrase number: 2
theeaglehaslande

iterative attempt:
NO, cannot be split

memoized attempt:
NO, cannot be split
"""

import sys

# dictionary is O(1) compared to searching through the dict each time O(n)
wdict = open("diction10k.txt", 'r')
W = set(wdict.read().splitlines())

def isWord(word: str)->bool:
    """ Loops over the given file to see if string is in it

    Args:
        word (str): string to check if is in word file

    Returns:
        bool: True if the word is in the file, and False if not
    """
    return word in W

def iterative(string: str):
    """Use a bottom up approact to see if the string can be broken up

    Args:
        string (str): string we will try  to split

    Resource:
        https://www.youtube.com/watch?v=WepWFGxiwRs
        https://stackoverflow.com/questions/37211678/how-to-create-the-matrix-axb-in-python
    """
    string_size = len(string) + 1
    result = [False for x in range(string_size)] # List to keep track of if the word is splittable or not
    split = [-1 for y in range(string_size)] # Location index
    # Properly indexes string at beginning
    result[0] = True
    split[0] = 0

    for i in range(string_size):
        for j in range(string_size):
            prefix = j - 1
            # Check if the current string we are looking at is a word
            # also checking if the end of the word is true
            # This is the split(i)
            if (isWord(string[prefix:i]) and result[prefix]):
                result[i] = True
                split[i] = j
    # Grab the last item in S
    # If it is true, then the string can be split
    if (result[-1]): 
        retString = ""
        k = len(string)
        while k > 0:
            # Store the index of the split
            i = split[k]
            retString = string[i-1:k] + " " + retString
            k = i - 1
        return retString
    else:
        return ""

    

def solve_memo(string: str):
    memo = {}
    return memoized(string, memo)

def memoized(string: str, memo: dict):
    """ uses memoization to try to split up word

    Args:
        string (str): string we will try to split
        memo (dict): dictionary we will use to memoize

    Returns:
        list: list containing the broken up string
                empty list if it could not be broken up
    """
    result = []
    if string in memo: # First check if string has already be memoized
        return memo.get(string)
    for i in range(len(string) + 1): # Loop over entire string
        substring = string[:i] # Break word up into prefixes that we can then search for
        # print(substring)
        if (isWord(substring)): # Check if the substring is a word
            if substring == string: # Check if substring == the word
                result.append(substring)
            else:
                # recursivly call solve, but increase index by 1 which shortens the string
                for phrase in memoized(string[i:], memo): 
                    result.append(substring + " " + phrase)
    memo[string] = result
    return result

def main():
    lines = []
    i = 1
    for line in sys.stdin:
        stripped = line.strip()
        if not stripped: break
        lines.append(stripped)
    word_total = int(lines.pop(0))

    while i <= word_total:
        word = lines[i-1]
        print(f'phrase number: {i}\n{word}\n')

        print("iterative attempt:")
        iterative_attempt = iterative(word)
        if(len(iterative_attempt) > 0):
            print("YES, can be split")
            print(iterative_attempt + "\n")
        else:
            print("NO, cannot be split\n")

        print("memoized attempt:")
        memoized_attempt = solve_memo(word)
        if(len(memoized_attempt) > 0):
            print("YES, can be split")
            print(memoized_attempt[0])
            print("\n")
        else:
            print("NO, cannot be split\n\n")
        i+=1
    wdict.close()

if __name__ == "__main__":
    main()
