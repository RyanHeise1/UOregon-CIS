"""Live Coding 2/17/21"""

from typing import Callable, List
from math import sqrt

def total_sum(list: List[int]) -> int:
    ct = 0
    for item in list:
        ct += item
    return ct

def apply(func: Callable, list: List[int]) -> List[int]:
    new_lst = []
    for item in list:
        new_lst.append(func(item))
    return new_lst

def square(list:List[int]) -> List[int]:
    return apply(lambda item: item ** 2, list)

def magnitude(list:List[int]) -> int:
    squares = square(list)
    total = total_sum(squares)
    return sqrt(total)


dispatch_table = {1: total_sum, 2: square, 3: magnitude}

class FunctionDispatcher:

    def __init__(self, dispatch_table: dict):
        self.dispatch_table = dispatch_table

    def process_command(self, key: int, list: List[int]):
        return self.dispatch_table[key](list)


if __name__ == "__main__":
    fd = FunctionDispatcher(dispatch_table)
    total = fd.process_command(1, [3, 4])
    print(total)
    sq = fd.process_command(2, [3, 4])
    print(sq)
    mag = fd.process_command(3, [3, 4])
    print(mag)