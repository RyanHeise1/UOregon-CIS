from logging import currentframe


class Node(object):
    def __init__(self, data):
        self.parent = None
        self.left = None
        self.right = None
        self.data = data


class Tree(object):
    """[summary]


    Attributes
    ----------

    Methods
    -------
    print()
        Print the data of all nodes in order
    __print()
        Recursively print a subtree (in order), rooted at curr_node
    insert()
        Inserts Node at correct position
    __insert()
        Recursively checks if data trying to insert is < or > than the current node its looking at
    min()
        Returns the minimum value held in the tree
    max()
        Returns the maximum value held in the tree
    __find_node()
        Returns the node in the tree. None if not found
    contains()
        Returns True/False that the node is in the tree
    __iter__()
        Iterates over the tree in the given order
    inorder()
        prints the nodes in order
    preorder()
        Prints the Head of the node, then Left, Right
    postorder()
        Prints the Left subtree first, then Right, Head
    __traverse()
        Prints order of the tree based on the arguments given
    find_successor()
        Finds the successor of a given node. If Tree is empty, it raises a KeyError
    delete
        Deleted the Node
    """
    # Binary Search Tree
    # class constants
    PREORDER = 1
    INORDER = 2
    POSTORDER = 3

    def __init__(self):
        # Do not create any other private variables.
        # You may create more helper methods as needed.
        self.root = None

    def print(self):
        # Print the data of all nodes in order
        self.__print(self.root)

    def __print(self, curr_node):
        # Recursively print a subtree (in order), rooted at curr_node
        if curr_node is not None:
            self.__print(curr_node.left)
            print(str(curr_node.data), end=' ')  # save space
            self.__print(curr_node.right)

    def insert(self, data):
        # Find the right spot in the tree for the new node
        # Make sure to check if anything is in the tree
        # Hint: if a node n is null, calling n.getData() will cause an error
        if self.root is None:
            # print(f'Inserted {data} Into Root Node')
            self.root = Node(data)
        else:
            # Calling private function becuase this will SOMETIMES be recursive
            self.__insert(data, self.root)
    
    def __insert(self, data, current_node):
        if data < current_node.data:
            if current_node.left is None: # Insert Node in left pos
                # print(f'Inserted {data} Into Left')
                new_node = Node(data)
                current_node.left = new_node
                new_node.parent = current_node
            else: # Move down the left side of the tree
                self.__insert(data, current_node.left)
        elif data > current_node.data:
            if current_node.right is None: # Insert node in right pos
                # print(f'Inserted {data} Into Right')
                new_node = Node(data)
                current_node.right = new_node
                new_node.parent = current_node
            else: # Move down the right side of the tree
                self.__insert(data, current_node.right)
        else:
            print(f"ERROR: {data} Already In Tree")

    def min(self):
        # Returns the minimum value held in the tree
        # Returns None if the tree is empty
        current_node = self.root
        if self.root != None:
            while current_node.left != None:
                current_node = current_node.left
            return current_node.data
        else:
            return None

    def max(self):
        # Returns the maximum value held in the tree
        # Returns None if the tree is empty
        current_node = self.root
        if self.root != None:
            while current_node.right != None:
                current_node = current_node.right
            return current_node.data
        else:
            return None

    def __find_node(self, data):
        # returns the node with that particular data value else returns None
        current_node = self.root
        while current_node != None:
            # print(current_node.data)
            if data > current_node.data:
                current_node = current_node.right
            elif data < current_node.data:
                current_node = current_node.left
            else:
                return current_node
        return None

    def contains(self, data):
        # return True of node containing data is present in the tree.
        # otherwise, return False.
        # you may use a helper method __find_node() to find a particular node with the data value and return that node
        if self.__find_node(data) is not None:
            return True
        else:
            return False

    def __iter__(self):
        # iterate over the nodes with inorder traversal using a for loop
        return self.inorder()

    def inorder(self):
        # inorder traversal : (LEFT, ROOT, RIGHT)
        return self.__traverse(self.root, Tree.INORDER)

    def preorder(self):
        # preorder traversal : (ROOT, LEFT, RIGHT)
        return self.__traverse(self.root, Tree.PREORDER)

    def postorder(self):
        # postorder traversal : (LEFT, RIGHT, ROOT)
        return self.__traverse(self.root, Tree.POSTORDER)

    def __traverse(self, curr_node, traversal_type):
        # helper method implemented using generators
        # all the traversals can be implemented using a single method
        
        #Yield data of the correct node/s
        if curr_node is not None:
            if traversal_type == Tree.INORDER: # INORDER
                yield from self.__traverse(curr_node.left, 2)
                yield curr_node.data
                yield from self.__traverse(curr_node.right, 2)

            if traversal_type == Tree.PREORDER: # PREORDER
                yield curr_node.data
                yield from self.__traverse(curr_node.left, 1)
                yield from self.__traverse(curr_node.right, 1)

            if traversal_type == Tree.POSTORDER: # POSTORDER
                yield from self.__traverse(curr_node.left, 3)
                yield from self.__traverse(curr_node.right, 3)
                yield curr_node.data
        # print(f'Root: {curr_node} \t Traversal Type: {traversal_type}')

    def find_successor(self, data):
        # helper method to implement the delete method but may be called on its own
        # if the right subtree of the node is nonempty,then the successor is just 
        # the leftmost node in the right subtree.
        # If the right subtree of the node is empty,then go up the tree until a node that is
        # the left child of its parent is encountered. The parent of the found node will be the
        #successor to the initial node.
    
    	#Return object of successor if found else return None
        node = self.__find_node(data)
        if self.root is None:
            raise KeyError("ERROR: Tree is Empty")
        if node.right is not None: # Has right side
            # TODO FInd min in right subtree
            # go right
            successor = node.right
            # go left untill None
            while successor.left is not None:
                successor = successor.left
            return successor
        y = node.parent
        while y is not None and node == y.right:
            node = y
            y = y.parent
        # print(y)
        return y
    
    def noChildren(self, node: Node):
        return node.right is None and node.left is None

    def oneChild(self, node: Node):
        return bool(node.left) + bool(node.right) == 1

    def twoChild(self, node: Node):
        return node.right is not None and node.left is not None


    def delete(self, data):
        if self.root is None:
            raise KeyError("ERROR: The root of the tree is None")
            
        node = self.__find_node(data)
        node_parent = node.parent

        # The node has no children
        if self.noChildren(node): 
            # This if statement just sets the nodes parent's pointer to None
            if node.parent:
                if node_parent.data > node.data: # Right side of parent matches the data we want to delete
                    node_parent.left = None 
                else: # Left side of parent matches the data we want to delete
                    node_parent.right = None

        elif self.oneChild(node): # The node has one child
            if node.parent:
                # Find where the leaf node is and store it as leaf
                if bool(node.left): 
                    leaf = node.left
                else:
                    leaf = node.right
                # print(f'Leaf Node: {leaf.data} \nParent: {node_parent.data} \nCurrent Node: {node.data}')
                if node_parent.data < node.data:
                    node_parent.right = leaf
                    leaf.parent = node_parent
                else:
                    node_parent.left = leaf
                    leaf.parent = node_parent
            else:
                if bool(node.right):
                    right = node.right
                    node.data = right.data
                    self.delete(right.data)
                else:
                    left = node.left
                    node.data = left.data
                    self.delete(left.data)

        elif self.twoChild(node): # 2 children
            successor = self.find_successor(node.data)
            successor_data = successor.data
            self.delete(successor.data)
            node.data = successor_data
        return
