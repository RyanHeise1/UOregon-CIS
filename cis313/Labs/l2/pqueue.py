import mheap

class pqueue(object):
    def __init__(self,size) :
        # Build the Constructor
        self.pheap = mheap.max_heap(size)


    def get_pqueue(self):
        # returns the current status of the queue
        return self.pheap.get_heap()
    
    def __iter__(self):
        return self.__traverse()
    
    def __traverse(self):
        for i in range(self.pheap.length):
            yield self.pheap.heap[i]

    def insert(self, data):
        # insert and rearrange the queue based on the priority
        self.pheap.insert(data)
    
    def peek(self):
        # return the highest priority from the priority queue. No need to remove
        return self.pheap.peek()
    
    def extract_max(self):
        # remove and return the highest priority from the priority queue
        return self.pheap.extract_max()

    def is_empty(self):
        # Return true when the priority queue is empty
        return not bool(self.pheap.length)


