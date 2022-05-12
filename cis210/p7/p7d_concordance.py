'''
Title - Concordance

Author - Ryan Heise

Credit - N/A
'''

import string


def fconcordance(fname):
    '''
    (fname: str) -> dict

    This function goes over a file and finds where the words occur at each line

    >>> fconcordance('tinyf.txt')
    'hello' occurs in lines: [1, 3]
    'world' occurs in lines: [2]
    '''
    # Dictionary to keep track of occurences
    mydict = {}
    with open(fname, 'r') as file:
        # line number
        line_num = 1

        # For loop to iterate over each line for file
        for line in file:
            line = line.lower().strip()
            for c in string.punctuation:
                line = line.replace(c,"")

                
            # Split line in word
            mylist = line.split()

            # Iterare over each word in file
            for word in mylist:
                # If the word is not in dictionary
                if word not in mydict.keys():
                    # add word to dictionary with its line num
                    mydict[word] = [line_num]
                else:
                    # Else append to list
                    mydict[word].append(line_num)

                # Increase line number
                line_num += 1

    return mydict


def main():
    '''
    () -> None

    Main auxilary function

    >>> main()
    'hello' occurs in lines: [1, 3]
    'world' occurs in lines: [2]
    '''
    mydict = fconcordance('tinyf.txt')
    for key, value in mydict.items():
        print('\'' + key + '\'', 'occurs in lines:', value)

if __name__ == "__main__":
    main()
