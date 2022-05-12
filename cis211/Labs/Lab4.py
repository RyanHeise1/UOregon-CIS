"""
Live Coding Done In Lab 1/27/21

Project - Binary Search Tree
"""

class Node:
    def __init__(self, val: int):
        self.node_data = val

    def sum_node_data(self):
        raise NotImplementedError("Im lonely. Please Implement me!")

    def __str__(self):
        return f"<{self.node_data},>"


class Leaf(Node):
    def __init__(self, node_data: int):
        self.node_data = node_data

    def sum_node_data(self):
        return self.node_data


class Internal(Node):
    def __init__(self, node_data: int, left: "Node", right: "Node"):
        """
        left and right are the left and right children of an internal node
        """
        self.node_data = node_data
        self.left = left
        self.right = right

    def sum_node_data(self):
        left_val = self.left.sum_node_data()
        right_val = self.right.sum_node_data()
        return left_val + right_val + self.node_data



def main():
    l1 = Leaf(3)
    l2 = Leaf(6)
    l3 = Leaf(9)
    i = Internal(7, l2, l3)
    root = Internal(5, l1, i)
    print(root.sum_node_data())

if __name__ == '__main__':
    main()