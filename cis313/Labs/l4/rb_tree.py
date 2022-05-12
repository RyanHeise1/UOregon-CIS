class Node(object):
    def __init__(self, data, left = None, right = None, parent = None, color = 'red'):
        self.data = data
        self.left = left
        self.right = right
        self.parent = parent
        self.color = color


class rb_tree(object):

    PREORDER = 1
    INORDER = 2
    POSTORDER = 3
    # initialize root and size
    def __init__(self):
        self.root = None
        self.sentinel = Node(None, color = 'black')
        self.sentinel.parent = self.sentinel
        self.sentinel.left = self.sentinel
        self.sentinel.right = self.sentinel
    
    def print_tree(self):
        # Print the data of all nodes in order
        self.__print_tree(self.root)
    
    def __print_tree(self, curr_node):
        # Recursively print a subtree (in order), rooted at curr_node
        # Printed in preorder
        if curr_node is not self.sentinel:
            print(str(curr_node.data), end=' ')  # save space
            self.__print_tree(curr_node.left)
            self.__print_tree(curr_node.right)

    def __print_with_colors(self, curr_node):
        # Recursively print a subtree (in order), rooted at curr_node
        # Printed in PREORDER
        # Extracts the color of the node and print it in the format -dataC- where C is B for black and R for red
        if curr_node is not self.sentinel:
        
            if curr_node.color == "red":
                node_color = "R"
            else:
                node_color = "B"

            print(str(curr_node.data)+node_color, end=' ')  # save space
            self.__print_with_colors(curr_node.left)
            self.__print_with_colors(curr_node.right)

    def print_with_colors(self):
        # Also prints the data of all node but with color indicators
        self.__print_with_colors(self.root)
            
            
    def __iter__(self):
        return self.inorder()

    def inorder(self):
        return self.__traverse(self.root, rb_tree.INORDER)

    def preorder(self):
        return self.__traverse(self.root, rb_tree.PREORDER)

    def postorder(self):
        return self.__traverse(self.root, rb_tree.POSTORDER)

    def __traverse(self, curr_node, traversal_type):
        if curr_node is not self.sentinel:
            if traversal_type == self.PREORDER:
                yield curr_node
            yield from self.__traverse(curr_node.left, traversal_type)
            if traversal_type == self.INORDER:
                yield curr_node
            yield from self.__traverse(curr_node.right, traversal_type)
            if traversal_type == self.POSTORDER:
                yield curr_node

    # find_min travels across the left of every node, and returns the
    # node who has no left. This is the min value of a subtree
    def find_min(self):
        current_node = self.root
        while current_node.left:
            current_node = current_node.left
        return current_node
    
    # find_node expects a data and returns the Node object for the given data
    def find_node(self, data):
        if self.root:
            res = self.__get(data, self.root)
            if res:
                return res
            else:
                raise KeyError('Error, data not found')
        else:
            raise KeyError('Error, tree has no root')

    # helper function __get receives a data and a node. Returns the node with
    # the given data
    def __get(self, data, current_node):
        if current_node is self.sentinel: # if current_node does not exist return None
            print("couldnt find data: {}".format(data))
            return None
        elif current_node.data == data:
            return current_node
        elif data < current_node.data:
            # recursively call __get with data and current_node's left
            return self.__get( data, current_node.left )
        else: # data is greater than current_node.data
            # recursively call __get with data and current_node's right
            return self.__get( data, current_node.right )
    

    def find_successor(self, data):
        # Private Method, can only be used inside of BST.
        current_node = self.find_node(data)

        if current_node is self.sentinel:
            raise KeyError

        # Travel left down the rightmost subtree
        if current_node.right:
            current_node = current_node.right
            while current_node.left is not self.sentinel:
                current_node = current_node.left
            successor = current_node

        # Travel up until the node is a left child
        else:
            parent = current_node.parent
            while parent is not self.sentinel and current_node is not parent.left:
                current_node = parent
                parent = parent.parent
            successor = parent

        if successor:
            return successor
        else:
            return None

    # put adds a node to the tree
    def insert(self, data):
        # if the tree has a root
        if self.root:
            # use helper method __put to add the new node to the tree
            new_node = self.__put(data, self.root)
            self.__rb_insert_fixup(new_node)
        else: # there is no root
            # make root a Node with values passed to put
            self.root = Node(data, parent = self.sentinel, left = self.sentinel, right = self.sentinel)
            new_node = self.root
            self.__rb_insert_fixup(new_node)
    
    #Insertion for Binary Search Tree
    def bst_insert(self, data):
        # if the tree has a root
        if self.root:
            # use helper method __put to add the new node to the tree
            self.__put(data, self.root)
        else: # there is no root
            # make root a Node with values passed to put
            self.root = Node(data, parent = self.sentinel, left = self.sentinel, right = self.sentinel)
        
    # helper function __put finds the appropriate place to add a node in the tree
    def __put(self, data, current_node):
        if data < current_node.data:
            if current_node.left != self.sentinel:
                new_node = self.__put(data, current_node.left)
            else: # current_node has no child
                new_node = Node(data,
                parent = current_node,
                left = self.sentinel,
                right = self.sentinel )
                current_node.left = new_node
        else: # data is greater than or equal to current_node's data
            if current_node.right != self.sentinel:
                new_node = self.__put(data, current_node.right)
            else: # current_node has no right child
                new_node = Node(data,
                parent = current_node,
                left = self.sentinel,
                right = self.sentinel )
                current_node.right = new_node
        return new_node

    
    def delete(self, data):
        # Same as binary tree delete, except we call rb_delete fixup at the end.

        z = self.find_node(data)
        if z.left is self.sentinel or z.right is self.sentinel:
            y = z
        else:
            y = self.find_successor(z.data)
        
        if y.left is not self.sentinel:
            x = y.left
        else:
            x = y.right
        
        if x is not self.sentinel:
            x.parent = y.parent

        if y.parent is self.sentinel:
            self.root = x

        else:
            if y == y.parent.left:
                y.parent.left = x
            else:
                y.parent.right = x
        
        if y is not z:
            z.data = y.data
    
        if y.color == 'black':
            if x is self.sentinel:
                self.__rb_delete_fixup(y)
            else:
                self.__rb_delete_fixup(x)

    def left_rotate(self, current_node):
        # if x is the root of the tree to rotate with left child subtree T1 and right child y, 
        # where T2 and T3 are the left and right children of y then:

        # x becomes left child of y and T3 as its right child of y
        # T1 becomes left child of x and T2 becomes right child of x
        # refer page 312 of CLRS book for rotations
        if current_node is None or self.root is None:
            raise KeyError(f'ERROR: No Node {current_node}')
        y = current_node.right  # set y
        current_node.right = y.left  # turn y's left subtree into current trees's right subtree
        if y.left != self.sentinel:
            y.left.parent = current_node
        y.parent = current_node.parent  # link x's parent to y
        if current_node.parent == self.sentinel:
            self.root = y
        elif current_node == current_node.parent.left:
            current_node.parent.left = y
        else:
            current_node.parent.right = y
        y.left = current_node  # put x on y's left
        current_node.parent = y
    
    def right_rotate(self, current_node):
        # If y is the root of the tree to rotate with right child subtree T3 and left child x, 
        # where T1 and T2 are the left and right children of x then:
        # y becomes right child of x and T1 as its left child of x
        # T2 becomes left child of y and T3 becomes right child of y
        # refer page 312 of CLRS book for rotations
        if current_node is None or self.root is None:
            raise KeyError(f'ERROR: No Node {current_node}')
        x = current_node.left
        current_node.left = x.right
        if x.right != self.sentinel:
            x.right.parent = current_node
        x.parent = current_node.parent
        if current_node.parent == self.sentinel:
            self.root = x
        elif current_node == current_node.parent.right:
            current_node.parent.right = x
        else:
            current_node.parent.left = x
        x.right = current_node
        current_node.parent = x

    
    def __rb_insert_fixup(self, z):
        # This function maintains the balancing and coloring property after bst insertion into
        # the tree. Please red the code for insert() method to get a better understanding
        # refer page 316 of CLRS book and lecture slides for rb_insert_fixup
        while z.parent.color == "red":
            if z.parent == z.parent.parent.left:
                y = z.parent.parent.right
                if y.color == "red":  # case 1
                    z.parent.color = "black"        
                    y.color = "black"
                    z.parent.parent.color = "red"   
                    z = z.parent.parent             
                else:
                    if z == z.parent.right:  # case 2
                        z = z.parent
                        self.left_rotate(z)

                    z.parent.color = "black"  # case 3
                    z.parent.parent.color = "red"
                    self.right_rotate(z.parent.parent)

            else:
                if z.parent == z.parent.parent.right:
                    y = z.parent.parent.left
                    if y.color == "red":  # case 1
                        z.parent.color = "black"
                        y.color = "black"
                        z.parent.parent.color = "red"
                        z = z.parent.parent
                    else:
                        if z == z.parent.left:  # case 2
                            z = z.parent                
                            self.right_rotate(z)         

                        z.parent.color = "black"  # case 3
                        z.parent.parent.color = "red"
                        self.left_rotate(z.parent.parent)
        self.root.color = "black"

    def __rb_delete_fixup(self, x):
        # This function maintains the balancing and coloring property after bst deletion 
        # from the tree. Please read the code for delete() method to get a better understanding.
        # refer page 326 of CLRS book and lecture slides for rb_delete_fixup
        while x != self.root and x.color == "black":
            if x == x.parent.left:
                w = x.parent.right
                if w.color == "red":
                    w.color = "black"  # case 1
                    x.parent.color = "red"  # case 1
                    self.left_rotate(x.parent)  # case 1
                    w = x.parent.right  # case 1
                if w.left.color == "black" and w.right.color == "black":
                    w.color = "red"  # case 2
                    x = x.parent  # case 2
                elif w.right.color == "black":
                    w.left.color = "black"  # case 3
                    w.color = "red"  # case 3
                    self.right_rotate(w)  # case 3
                    w = x.parent.right  # case 3
                else:
                    w.color = x.parent.color  # case 4
                    x.parent.color = "black"  # case 4
                    w.right.color = "black"  # case 4
                    self.left_rotate(x.parent)  # case 4
                    x = self.root  # case 4
            # Code below is the same as above, "left" and "right" exchanged
            else:                                   
                w = x.parent.left
                if w.color == "red":
                    w.color = "black"
                    x.parent.color = "red"
                    self.right_rotate(x.parent)
                    w = x.parent.left
                if w.right.color == "black" and w.left.color == "black":
                    w.color = "red"
                    x = x.parent
                elif w.left.color == "black":
                    w.right.color = "black"
                    w.color = "red"
                    self.left_rotate(w)
                    w = x.parent.left
                else:
                    w.color = x.parent.color
                    x.parent.color = "black"
                    w.left.color = "black"
                    self.right_rotate(x.parent)
                    x = self.root
        x.color = "black"
