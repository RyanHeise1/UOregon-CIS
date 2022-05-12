import lab1
import unittest

class T0_TestingQueue(unittest.TestCase):

    def test_basic_enqueue(self):
        # testing the basic enqueue operation
        print("\n")
        q = lab1.Queue()
        q.enqueue(1)
        q.enqueue(2)
        q.enqueue(3)
        q.enqueue(4)

        self.assertEqual(q.__str__(), '[1, 2, 3, 4]')
        print("\n")

class T1_TestingStack(unittest.TestCase):

    def test_is_empty_false(self):
        # testing if queue is empty
        print("\n")
        s = lab1.Stack()
        s.push("4")
        print("return false if the stack is not empty")
        self.assertEqual(s.isEmpty(), False)
        print("\n")


class T2_TestingPalindrome(unittest.TestCase):

    def test_basic_string(self):
        # testing with basic string
        print("\n")
        string = "hello"
        p = lab1.isPalindrome(string)
        print("The string being tested is -> ", string)
        self.assertEqual(p, False)
        print("\n")

if __name__ == '__main__':
    unittest.main()
