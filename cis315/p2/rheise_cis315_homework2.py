"""
Author: Ryan Heise
UO CIS 315W 2022
"""

def paths(edg_lst: list, visited: list, result: list, node: int, curr_index: int, curr_length: int):
    """ This program takes in a description of a weighted directed acyclic graph from standard input.
    (See inSample2.txt for an example of input) The goal of this program is (i) to counts the longest 
    path from Node 1 to Node N and (ii) the number of distinct paths from Node 1 to Node N that have 
    the longest length [found in (i)]

    This program runs in linear time O(n + m) 
    where n is the number of nodes and m is the number of edges

    Args:
        visited (list): Visited nodes
        edg_lst (list): List of all edges
        result (list): List of the paths
        node (int): Number of nodes
        curr_index (int): Current index
        curr_len (int): Current length
     
    Returns:
        None: Answer will be stored in result
    """
    if (curr_index != node):
        # Loop and visit all nodes
        for i in range(len(edg_lst[curr_index])):
            # if node has not been visited
            if(not visited[edg_lst[curr_index][i][0]]):
                # mark current node as visited (True)
                visited[edg_lst[curr_index][i][0]] = True
                # Recursivly call dfs
                paths(edg_lst, visited, result, node, edg_lst[curr_index][i][0], curr_length + edg_lst[curr_index][i][1])
                visited[edg_lst[curr_index][i][0]] = False
    else:
        # curr_index = node
        # print(f'Result[0]: {result[0]} \tCurr_len: {curr_length}')
        if(result[0] == curr_length):
            result[1] += 1
        elif(result[0] < curr_length):
            result[0] = curr_length
            result[1] = 1
        return


def main():
    """Main function that runs path()

    Returns:
        None: Prints out (i) longest path and (ii) number of longest paths 
    """
    # Keep track of visited nodes
    visited = []
    # Store all edges
    edg_lst = []
    result = [0, 0]
    curr_index = 0
    curr_length = 0

    # Split up first line of input to get V and edges
    line = input().split(' ')
    node = int(line[0])
    edges = int(line[1])
    #print(f'node: {node}\ne: {edges}')
    
    # Create base lists for the number of ves in the matrix
    for elements in range(node):
        edg_lst.append([])
        # Set visited status of all nodes to false
        visited.append(False)

    # Loop and append all edges in the input file
    for item in range(edges):
        # Split line so we can store [source, destination, weight]
        line = input().split(' ')
        source = int(line[0])
        destination = int(line[1])
        weight = int(line[2])
        # Sub 1 from source and dest becuase they start at 1 (want list to start at 0)
        edg_lst[source - 1].append((destination-1, weight))
    #print(edg_lst)
    
    # Run DFS to find longest path and # of longest paths
    paths(edg_lst, visited, result, node-1, curr_index, curr_length)
    print(f'longest path: {result[0]}\nnumber of longest paths: {result[1]}')


if __name__ == "__main__":
    main()