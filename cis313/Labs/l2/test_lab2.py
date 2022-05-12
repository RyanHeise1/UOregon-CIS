import unittest
import pqueue
import mheap


class T0_pqueue_insert(unittest.TestCase):


    def test_1_pq_insert(self):
        print("\n")
        pq = pqueue.pqueue(5)
        pq.insert(1)
        pq.insert(2)
        pq.insert(3)
        pq.insert(4)
        pq.insert(5)
        pq_list = [element for element in pq]
        self.assertEqual(pq_list, [5,4,2,1,3])
        print("\n")


class T1_pqueue_peek(unittest.TestCase):

    def test_1_pq_peek(self):
        print("Just return the fist element of the queue.")
        print("\n")
        pq = pqueue.pqueue(5)
        pq.insert(1)
        pq.insert(2)
        pq.insert(3)
        self.assertEqual(pq.peek(), 3)
        print("\n")


class T2_pqueue_extract_max(unittest.TestCase):

    def test_1_pq_extract_max(self):
        print("extract and return the maximum element of the queue")
        print("\n")
        pq = pqueue.pqueue(5)
        pq.insert(1)
        pq.insert(2)
        pq.insert(3)
        self.assertEqual(pq.extract_max(), 3)
        print("\n")


class T5_heap_sort(unittest.TestCase):
    def test_heap_sort_1(self):
        print("\n")
        to_sort_list = [10,24,3,57,4,67,37,87,7]
        sorted_list = mheap.heap_sort(to_sort_list)
        self.assertEqual(sorted_list, [3, 4, 7, 10, 24, 37, 57, 67, 87])
        print("\n")


class T6_self_checks(unittest.TestCase):
    def test_build_heap(self):
        print("Check that build_heap() properly builds a max heap.")
        print("\n")
        x = [10,5,4,6,7,3]
        heap = mheap.max_heap(len(x),x)
        heap.build_heap()
        self.assertEqual(heap.get_heap(), [10,7,4,6,5,3])

    def test_index_error(self):
        print("Check that an IndexError is raised when inserting to a heap that is already full.")
        print("\n")
        pq = pqueue.pqueue(3)
        pq.insert(1)
        pq.insert(2)
        pq.insert(3)
        with self.assertRaises(IndexError):
            pq.insert(4)

    def test_index_error(self):
        print("Check that a KeyError is raised when extract_max() is called on an empty heap.")
        print("\n")
        pq = pqueue.pqueue(3)
        with self.assertRaises(KeyError):
            pq.extract_max()

    def test_textttpqueue(self):
        print("Check that an object of textttpqueue is still a valid maximum heap after calling insert() and extract_max().")
        print("\n")
        pq = pqueue.pqueue(5)
        pq.insert(50)
        pq.insert(100)
        pq.insert(383)
        pq.insert(5)
        pq.insert(25)
        pq.extract_max()
        lst = pq.get_pqueue()
        self.assertEqual(lst, [100, 50, 25, 5, None])

    def test_mult_extract(self):
        print("Tests that extract max works and fills the voided value with None type")
        print("\n")
        to_sort_list = [10,5,4,6,7,3]
        heap = mheap.max_heap(len(to_sort_list),to_sort_list)
        heap.build_heap()
        heap.extract_max()
        heap.extract_max()
        heap.extract_max()
        self.assertEqual(heap.get_heap(), [5, 3, 4, None, None, None])
    
    def test_adding_non_int(self):
        print("Tests that insert allows strings")
        pq = pqueue.pqueue(3)
        pq.insert("string")
        self.assertEqual(pq.get_pqueue(), ['string', None, None])





    
    
if __name__ == '__main__':
    unittest.main()