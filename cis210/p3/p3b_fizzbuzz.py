'''
Title: Project 3-b - fizzbuzz 

Author: Ryan Heise

Credits: N/A
'''
def fb(n):
    '''
    Figures out if number is fizz, buzz, or fizzbuzz

    parameters:
        n: user entered number that will be used in the for loop

    reutrns None
    Prints if number is fizz, buzz, or fizzbuzz and exits loop when done
    and prints game over

    >>> fb(15)
    1
    2
    fizz
    4
    buzz
    fizz
    7
    8
    fizz
    buzz
    11
    fizz
    13
    14
    fizzbuzz
    Game over!
    '''
    for num in range(1, n + 1):
        if num % 3 == 0 and num % 5 == 0:
            print("fizzbuzz")
        elif num % 3 == 0:
            print("fizz")
        elif num % 5 == 0:
            print("buzz")
        else:
            print(num)
    print("Game Over!")
    return

def main():
    '''
    prompts user to enter number

    calls fb(n) with number entered

    returns None

    >>> main()
    Please enter a positive integer to start fizzbuzz game: 3
    1
    2
    fizz
    Game over!
    >>>
    '''
    number = int(input("Please enter a postitive integer to start fizzbuzz game: "))
    fb(number)
    return
