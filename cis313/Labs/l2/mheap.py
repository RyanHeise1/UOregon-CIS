class max_heap(object):
    """ Binary max-heap

    Supports most standard heap operations (insert, peek, extract_max).
    Can be used for building a priority queue or heapsort. Since Python
    doesn't have built-in arrays, the underlying implementation uses a
    Python list instead. When initialized, max_heap creates a new list of
    fixed size or uses an existing list.

    Attributes
    ----------
        size (int): size of the heap
        data (array): array that holds the values in the heap
        
    
    Methods
    -------
    get_heap()
        returns the heap that is given
    insert(data)
        inserts data into the heap
    peek()
        looks at the top node in the heap
    extract_max()
        extracts max value from the node by replacing it with the last value
    __heapify(curr_index, list_length = None)
        moves top elem down the heap at curr_index
    build_heap()
        builds max heap
    __get_parent(loc)
        gets parent of node/ leaf at loc
    __get_left(loc)
        gets left node at loc
    __get_right(loc)
        gets right node at loc
    __swap(a, b)
        swaps a and b node/ leaf in the heap
    __check_full()
        chechs if the heap is full
    """

    def __init__(self, size = 20, data = None):
        """Initialize a binary max-heap.

        size: Total capacity of the heap.
        data: List containing the desired heap contents. 
              The list is used in-place, not copied, so its contents 
              will be modified by heap operations.
              If data is specified, then the size field is ignored."""

        # Add to this constructor as needed
        if data is not None:
            self.max_size = len(data)
            self.length = len(data)
            self.heap = data
        else:
            self.max_size = size
            self.length = 0
            self.heap = [None] * size
        
    def get_heap(self):
        return self.heap

    def insert(self, data):
        """Insert an element into the heap.

        Raises IndexError if the heap is full."""
        if self.__check_full():
            raise IndexError("ERROR: Heap Is Full")
        self.length += 1
        # Insert data into the heap
        self.heap[self.length - 1] = data
        index = self.length - 1
        #print(f'Value Appended: {self.heap[index]} Index: {index}')
        #print(f'{index >= 1} and {self.heap[index] > self.heap[self.__get_parent(index)]}')
        while (index >= 1 and self.heap[index] > self.heap[self.__get_parent(index)]):
            # print(f'Swap {self.heap[index]} and {self.heap[self.__get_parent(index)]}')
            self.__swap(index, self.__get_parent(index))
            index = self.__get_parent(index)

    def peek(self):
        """Return the maximum value in the heap."""
        if self.length == 0:
            return None
        return self.extract_max()

    def extract_max(self):
        """Remove and return the maximum value in the heap.

        Raises KeyError if the heap is empty."""
        # Tips: Maximum element is first element of the list
        #     : swap first element with the last element of the list.
        #     : Remove that last element from the list and return it.
        #     : call __heapify to fix the heap
        if self.__check_full() or self.length == 0:
            raise KeyError("ERROR: Heap Is Full")
        heap = self.get_heap()
        # Store max value to return later
        max_val = heap[0]
        # Swap max val with bottom value
        self.__swap(0, self.length - 1)
        heap[self.length - 1] = None
        # Remove max from heap
        self.length -= 1
        # Move top value down the heap
        self.__heapify(0, self.length)
        return max_val

    def __heapify(self, curr_index, list_length = None):
        """Moves elements down in the heap

        Args:
            curr_index (int): current index to move down
            list_length ([int, optional): length of the list. Defaults to None.
        """
        # helper function for moving elements down in the heap
        # Page 157 of CLRS book
        heap = self.get_heap()
        left = self.__get_left(curr_index)
        right = self.__get_right(curr_index)

        if left <= self.length - 1 and heap[left] > heap[curr_index]:
            largest = left
        else:
            largest = curr_index

        if right <= self.length - 1 and heap[right] > heap[largest]:
            largest = right

        if largest != curr_index:
            self.__swap(curr_index, largest)
            self.__heapify(largest)



    def build_heap(self):
        """ 
        Builds max heap from the list l. Starts at the bottom and works its way up
        """
        # builds max heap from the list l.
        # Tip: call __heapify() to build to the list
        #    : Page 157 of CLRS book
        size = self.length
        for i in range((self.length // 2), -1, -1):
            self.__heapify(i, size)


    ''' Optional helper methods may be used if required '''
    ''' You may create your own helper methods as required.'''
    ''' But do not modify the function definitions of any of the above methods'''

    def __get_parent(self, loc):
        # left child has odd location index
        # right child has even location index
        # if loc % 2 == 0:
        #     parent = int((loc - 2) / 2)
        # else:
        parent = int((loc - 1) / 2)
        return parent

    def __get_left(self, loc):
        return 2*loc + 1

    def __get_right(self, loc):
        return 2*loc + 2

    def __swap(self, a, b):
        # swap elements located at indexes a and b of the heap
        temp = self.heap[a]
        self.heap[a] = self.heap[b]
        self.heap[b] = temp

    def __check_full(self):
        return self.length > self.max_size
    



    

def heap_sort(l):
    """Sort a list in place using heapsort."""
    # Note: the heap sort function is outside the class
    #     : The sorted list should be in ascending order
    # Tips: Initialize a heap using the provided list
    #     : Use build_heap() to turn the list into a valid heap
    #     : Repeatedly extract the maximum and place it at the end of the list
    #     : Refer page 161 in the CLRS textbook for the exact procedure
    length = len(l)
    max_h = max_heap(length, l)
    max_h.build_heap()
    sort = [max_h.extract_max() for i in range(length, 0 , -1)]
    # Invert the list
    return sort[::-1]
