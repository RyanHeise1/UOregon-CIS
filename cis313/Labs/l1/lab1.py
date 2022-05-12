class Node(object):
    """
    A class to represent a node.

    ...

    Attributes
    ----------
    data : int or float
        An individual character or number to be stored in a node
    next_node : object of class Node
        A pointer to the next node in a stack or queue

    Methods
    -------
    setData(data):
        Updates the value of data attribute of Node
    setNext(next_node):
        Updates the value of next_node attribute of Node
    getData():
        Returns the value of data attribute
    getNext():
        Returns the value of next_node attribute
    """
    def __init__(self, data = None, next_node = None):
        """
        Constructs (or initializes) the attributes for an object of the class

        ...

        Parameters
        ----------
        data : int or float
            An individual character or number to be stored in a node
        next_node : object of class Node
            A pointer to the next node in a stack or queue

        """
        self.__data = data
        self.__next_node = next_node

    def setData(self, data):
        '''Set the "data" data field to the corresponding input.'''
        self.__data = data

    def setNext(self, next_node):
        '''Set the "next_node" data field to the corresponding input.'''
        self.__next_node = next_node

    def getData(self):
        '''Return the "data" data field.'''
        return self.__data

    def getNext(self):
        '''Return the "next_node" data field.'''
        return self.__next_node

class Queue(object):
    """
    A class to represent a Queue.

    ...

    Attributes
    ----------
    head : int or float or string
        The head of the Queue
    tail : object of class Node
        End of the Queue

    Methods
    -------
    enqueue(newData):
        Adds a new value to the tail of the queue
    dequeue():
        removes object from the head of the queue
    isEmpty():
        Returns True is queue is NOT empty, False if it IS empty
    """
    def __init__(self):
        """
        init method stores the head and tail values of the Queue

        ...

        Parameters
        ----------
        head : int or float or string
            An individual character or number to be stored in a node
        tail : object of class Node
            End of the Queue
        """
        self.__head = None
        self.__tail = None

    def __str__(self):
        '''Loop through your queue and print each Node's data.'''
        elem = []
        current = self.__head
        while current is not None:
            elem.append(current.getData())
            current = current.getNext()
        return (f'{elem}')

    def enqueue(self, newData):
        '''Create a new node whose data is newData and whose next node is null
        Update head and tail.'''
        node = Node(newData)
        # If Queue is empty
        if self.__tail == self.__head == None:
            # Setting head and tail to the new node
            self.__head = self.__tail = node
        # Not empty
        else:
            # Moving tail to None value
            self.__tail.setNext(node)
            self.__tail = node

    def dequeue(self):
        '''Return the head of the Queue
        Update head.'''
        if self.isEmpty():
            return
        head = self.__head.getData()
        self.__head = self.__head.getNext()
        return head

    def isEmpty(self):
        '''Check if the Queue is empty.'''
        return self.__head == self.__tail is None


class Stack(object):
    """
    A class to represent a Stack.

    ...

    Attributes
    ----------
    head : int or float or string
        The head of the Stack

    Methods
    -------
    push(newData):
        Adds a new value to the end of the queue
    pop():
        removes object from the head of the queue
    isEmpty():
        Returns True is queue is NOT empty, False if it IS empty
    """
    def __init__(self):
        """
        init method stores the head value of the Stack

        ...

        Parameters
        ----------
        head : int or float or string
            An individual character or number to be stored in a node
        """
        self.head = None

    def __str__(self):
        '''Loop through your stack and print each Node's data.'''
        lst = []
        head = self.head
        while head is not None:
            lst.append(head.getData())
            head = head.getNext()
        return f"{lst}"

    def push(self, newData):
        '''We want to create a node whose data is newData and next node is top.
        Push this new node onto the stack
        Update top'''
        node = Node()
        node.setData(newData)
        node.setNext(self.head)
        self.head = node

    def pop(self):
        ''' Return the Node that currently represents the top of the stack.
        Update top'''
        if self.isEmpty():
            return None
        else:
            pop_val = self.head.getData()
            self.head = self.head.getNext()
            return pop_val

    def isEmpty(self):
        '''Check if the Stack is empty.'''
        return self.head is None


def isPalindrome(s):
    '''Use your Queue and Stack class to test wheather an input is a palindrome.'''
    myStack = Stack()
    myQueue = Queue()

    for letter in s.lower():
        if letter == " ":
            continue
        myStack.push(letter)
        myQueue.enqueue(letter)
    result = myStack.__str__() == myQueue.__str__()
    return result

def isPalindromeEC(s):
    '''Implement if you wish to do the extra credit.'''

    # Return appropriate value
    return