"""
Title - Where is Waldo

Author - Ryan Heise

    loop from length of list
        start with first item in each list and add one each time
        move to 2nd... etc

"""
Waldo = 'W'
Other = '.'

# For all, there exists
def all_row_exists_waldo(l:list) -> bool:
    """
    For all rows in the matrix, Waldo is in some column
    """
    for item in l:
        if Waldo not in item:
            return False
    return True


def all_col_exists_waldo(l:list) -> bool:
    """
    For all columns in the matrix, Waldo is in some row
    """
    if len(l) == 0:
        return True
    # Starting at col 0
    for col in range(len(l[0])):
        # Setting the list to be empty evey time we finish looping through the col
        col_lst = []
        # Starting row at 0
        for row in range(len(l)):
            # The apends the row in col in the list (ie: first time it runs through it appends l[0][0], l[1][0],
            #       l[2][0], l[0][1]
            col_lst.append(l[row][col])
        # When it is done looping through the row it will hit the if statement to check if there is a Waldo in there
        #       which will return false
        if Waldo not in col_lst:
            return False
        # Looping through row is done and now it will increase the col and start on l[0][1] which means it's starting
        #       on col 1 instead of 0
    return True


# For all, for all
def all_row_all_waldo(l:list) -> bool:
    """
    For all rows in the matrix, Waldo is in every column
    """
    for item in l:
        if Other in item:
            return False
    return True


def all_col_all_waldo(l:list):
    """
    For all the columns in the matrix, Waldo is in every row

    This is the same loop as all_col_exists_waldo except we are checking if there is Other in the list of col
    """
    if len(l) == 0:
        return True
    for col in range(len(l[0])):
        col_lst = []
        for row in range(len(l)):
            col_lst.append(l[row][col])
        if Other in col_lst:
            return False
    return True


# There exists, there exists
def exists_row_exists_waldo(l:list):
    """
    There is some row in the matrix in which Waldo is in some column
    """
    if len(l) == 0:
        return False
    waldo_ct = 0
    for item in l:
        if Waldo in item:
            waldo_ct += 1
    if waldo_ct == 0:
        return False
    return True


def exists_col_exists_waldo(l:list):
    """
    There is some column in the matrix in which Waldo is in some row
    """
    if len(l) == 0:
        return False
    waldo_ct = 0
    for col in range(len(l[0])):
        col_lst = []
        for row in range(len(l)):
            col_lst.append(l[row][col])
        if Waldo in col_lst:
            waldo_ct += 1
    if waldo_ct == 0:
        return False
    return True


# There exists, for all
def exists_row_all_waldo(l:list):
    """
    There is some row in the matrix in which Waldo is in every column
    """
    for row in l:
        if len(row) == 0:
            return True
        if row.count(Waldo) == 3:
            return True
    return False



def exists_col_all_waldo(l:list):
    """
    There is some column in the matrix in which Waldo is in every row
    """
    if len(l) == 0:
        return False
    for col in range(len(l[0])):
        col_lst = []
        for row in range(len(l)):
            col_lst.append(l[row][col])
            if col_lst.count(Waldo) == 3:
                return True
    return False


