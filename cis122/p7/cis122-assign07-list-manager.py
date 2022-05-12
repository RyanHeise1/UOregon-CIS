'''
CIS 122 Winter 2020 Assignment 7
Author: Ryan Heise
Partner: None
Description: Add, delete, and clear items from a list
'''

def cmd_help():
    """Prints out the available commands 

This function will print out the commands by first putting them in a list, then it prints the
commands out using a for loop

Args:
   t(user input): If the user enters ? this function will run

Returns:
   returns the available commands
"""
    print("*** Available commands ***")
    commands = ["Add", "Del", "List", "Clear"]
    description = ["Add to list", "Delete information", "List information", "Clear list"]
    max_length = get_max_list_item_size(commands)
    for i in range (len(commands)):
        pad_right(commands[i], max_length + 5)
        print(description[i])
    print("Empty to exit")

def cmd_add(t):
    """Counts items in list

This function will ask for information to put inside the list, if the input is blank then it will break.
when a string is entered it will append the value and add 1 to the item_count

Args:
   t(user input): If the user enters ? this function will run

Returns:
   prints out how many items are in the list so far
"""
    # count the items in list
    item_count = 0
    #inf loop to see when user breaks the loop and also to count the items in the list
    while True:
        add_input = input("Enter information (empty to stop): ")
        if add_input == "":
            break
        else:
            t.append(add_input)
            item_count += 1
            print("Added, item count = ", item_count)
def cmd_delete(t):
    """deletes selected item in the list

This function will print out all the items in the list using a for loop. 
the for loop also incorperates a list counting system that will number
each item in the list

Args:
   t(user input): If the user enters ? this function will run

Returns:
   returns all the items in the list and asks what to delte
"""
    if len(t) == 0:
        print("All items deleted")
        return
    while True:
        if len(t) == 0:
            print("All items deleted")
            break
        for i in range (len(t)):
            pad_right(str(i), 2)
            print(t[i])
        input_num = input("Enter number to delete (empty to stop: ")
        if input == "":
            break
        else:
            input_num = int(input_num)
            t.pop(input_num)
def cmd_list(t):
    """lists items in the list

This function will print out the strings in the list. 
For items in the word it will print them out

Args:
   t(user input): If the user enters ? this function will run

Returns:
   returns the items in the list 
"""
    print("List contains", len(t), "item(s)")
    if len(t) > 0:
        for i in t:
            print(i)
def cmd_clear(t):
    """Clears items in list

This function will clear items in the list using the delete function

Args:
   t(user input): If the user enters ? this function will run

Returns:
   returns the length of the list
"""
    print(len(t), "item(s) removed, list empty")
    del t[:]
def get_max_list_item_size(t):
    length = 0
    for i in t:
        if len(i) > length:
            length = (len(i))
    return length

def pad_left(str, space):
    """adds padding on the left side

This function will add a specfic string and ammount of spacing to the word

Args:
   str(str): String
   space(): specific spacing

Returns:
   prints the string with spacing
"""
    print(str.rjust(space, " "))  

def pad_right(str, space):
    """adds padding on the left side

This function will add a specfic string and ammount of spacing to the word

Args:
   str(str): String
   space(): specific spacing

Returns:
   prints the string with spacing
"""
    print(str.ljust(space, " "), end = "")

def pad_string():
    """adds padding on the left side

This function will add a specfic string and ammount of spacing to the word

Args:
    None

Returns:
   prints the string with spacing
"""
    print(" ".zfill(10))

l = list()

while True:
    user = input("Enter a command (? for help): ").strip()
    if user == '':
        break
    elif user == '?':
        cmd_help()
    elif user == 'add':
        cmd_add(l)
    elif user == 'list':
        cmd_list(l)
    elif user == 'clear':
        cmd_clear(l)
    elif user == 'del':
        cmd_delete(l)