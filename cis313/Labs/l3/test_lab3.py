import lab3
import unittest



class T0_tree__insert(unittest.TestCase):

    def test_balanced_binary_search_tree(self):
        print("\n")
        print("tree_insert_with_individual_check")
        t = lab3.Tree()

        t.insert(4)
        t.insert(2)
        t.insert(6)
        t.insert(1)
        t.insert(3)
        t.insert(5)
        t.insert(7)

        #The following check is without using tree as an iterator (which uses inorder traversal)
        #So this function also does not check the implementation of the traversal function

        self.assertEqual(t.root.data, 4)
        self.assertEqual(t.root.left.data, 2)
        self.assertEqual(t.root.left.left.data, 1)
        self.assertEqual(t.root.left.right.data, 3)
        self.assertEqual(t.root.right.data, 6)
        self.assertEqual(t.root.right.left.data, 5)
        self.assertEqual(t.root.right.right.data, 7)

        print("\n")


class T1_min_and_max(unittest.TestCase):

    def test_min_and_max(self):
        print("\n")
        print("Checkin the min and the max functions")
        t = lab3.Tree()

        t.insert(4)
        t.insert(2)
        t.insert(6)
        t.insert(1)
        t.insert(3)
        t.insert(5)
        t.insert(7)

        minimum = t.min()
        self.assertEqual(minimum, 1)
        maximum = t.max()
        self.assertEqual(maximum, 7)

        print("\n")

class T2_Traversal(unittest.TestCase):

    def test_traversal(self):
        print("\n")
        print("Checking all the three traversals")
        t = lab3.Tree()

        t.insert(4)
        t.insert(2)
        t.insert(6)
        t.insert(1)
        t.insert(3)
        t.insert(5)
        t.insert(7)
        tree_iterator = [node for node in t]
        inorder = [node for node in t.inorder()]
        preorder = [node for node in t.preorder()]
        

        print("__iter__(): inorder traversal")
        self.assertEqual(tree_iterator, [1, 2, 3, 4, 5, 6, 7])
        print("inorder traversal")
        self.assertEqual(inorder, [1, 2, 3, 4, 5, 6, 7])
        print("preorder traversal")
        self.assertEqual(preorder, [4, 2, 1, 3, 6, 5, 7])
        print("\n")




class T3_successor(unittest.TestCase):

    def test_successor(self):
        print("\n")
        print("successor function")
        tree_success = lab3.Tree()
        tree_success.insert(8)
        tree_success.insert(3)
        tree_success.insert(10)
        tree_success.insert(1)
        tree_success.insert(6)
        tree_success.insert(4)
        tree_success.insert(7)
        tree_success.insert(14)
        tree_success.insert(13)

        easy_success = tree_success.find_successor(8).data
        medium_success = tree_success.find_successor(10).data
        tough_success = tree_success.find_successor(7).data

        self.assertEqual(easy_success, 10)
        self.assertEqual(medium_success, 13)
        self.assertEqual(tough_success, 8)

        print("\n")


class T4_delete(unittest.TestCase):

    def test_delete(self):
        print("\n")
        print("delete function")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)

        l1 = [node for node in t]
        t.delete(7)
        l2 = [node for node in t]
        t.delete(6)
        l3 = [node for node in t]
        t.delete(8)
        l4 = [node for node in t]
        t.delete(10)
        l5 = [node for node in t]



        self.assertEqual(l1, [1, 3, 4, 6, 7, 8, 10, 13, 14])
        self.assertEqual(l2, [1, 3, 4, 6, 8, 10, 13, 14])
        self.assertEqual(l3, [1, 3, 4, 8, 10, 13, 14])
        self.assertEqual(l4, [1, 3, 4, 10, 13, 14])
        self.assertEqual(l5, [1, 3, 4, 13, 14])

        print("\n")

class T5_contains(unittest.TestCase):

    def test_contains(self):
        print("\n")
        print("contains function")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)
        self.assertEqual(t.contains(13), True)
        self.assertEqual(t.contains(15), False)
        print("\n")

class T6_additional_tests(unittest.TestCase):
    def test_insert(self):
        print("\ninsert function")
        tree = lab3.Tree()
        tree.insert(8)
        tree.insert(3)
        tree.insert(10)
        tree.insert(1)
        tree.insert(6)
        tree.insert(4)
        tree.insert(7)
        tree.insert(14)
        tree.insert(13)

        self.assertEqual(tree.root.data, 8)
        self.assertEqual(tree.root.left.right.right.data, 7)
        print("\n")

    def test_private_traverse(self):
        print("\nsuccessor function")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)

        inorder = [node for node in t._Tree__traverse(t.root, t.INORDER)]
        preorder = [node for node in t._Tree__traverse(t.root, t.PREORDER)]
        postorder = [node for node in t._Tree__traverse(t.root, t.POSTORDER)]

        self.assertEqual(inorder, [1, 3, 4, 6, 7, 8, 10, 13, 14])
        self.assertEqual(preorder, [8, 3, 1, 6, 4, 7, 10, 14, 13])
        self.assertEqual(postorder, [1, 4, 7, 6, 3, 13, 14, 10, 8])
        print("\n")

    def test_private_find_node(self):
        print("\n__find_node")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)

        node1 = t._Tree__find_node(10).data
        node2 = t._Tree__find_node(7).data
        node3 = t._Tree__find_node(14).data

        self.assertEqual(node1, 10)
        self.assertEqual(node2, 7)
        self.assertEqual(node3, 14)
        print("\n")

    def test_contains(self):
        print("\ncontains function")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)

        node1 = t.contains(10)
        node2 = t.contains(98239847982387498273984792387984729364725983750384) # :)
        node3 = t.contains(14)

        self.assertEqual(node1, True)
        self.assertEqual(node2, False)
        self.assertEqual(node3, True)
        print("\n")

    def test_find_successor(self):
        print("\nsuccessor function")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)

        test1 = t.find_successor(8).data
        test2 = t.find_successor(10).data
        test3 = t.find_successor(7).data

        self.assertEqual(test1, 10)
        self.assertEqual(test2, 13)
        self.assertEqual(test3, 8)
        print("\n")

    def test_delete(self):
        print("\ndelete function")
        t = lab3.Tree()
        t.insert(8)
        t.insert(3)
        t.insert(10)
        t.insert(1)
        t.insert(6)
        t.insert(4)
        t.insert(7)
        t.insert(14)
        t.insert(13)

        t.delete(6)
        l1 = [node for node in t]
        t.delete(1)
        l2 = [node for node in t]
        t.delete(8)
        l3 = [node for node in t]

        self.assertEqual(l1, [1, 3, 4, 7, 8, 10, 13, 14])
        self.assertEqual(l2, [3, 4, 7, 8, 10, 13, 14])
        self.assertEqual(l3, [3, 4, 7, 10, 13, 14])
        print("\n")

    def test_key_error_delete(self):
        print("\ndelete error raised")
        t = lab3.Tree()
        with self.assertRaises(KeyError):
            t.delete(5)
        print("\n")

    def test_key_error_find_successor(self):
        print("find successor error raised")
        t = lab3.Tree()
        with self.assertRaises(KeyError):
            t.find_successor(5)
        print("\n")
        




if __name__ == '__main__' :
    unittest.main()