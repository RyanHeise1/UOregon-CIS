"""
Lab 2/10/21

& AND: Sets each bit to 1 if both bits are 1
| OR: Sets each bit to 1 if one of two bits is 1
"""
from typing import List


class BinaryNumber(List):
    def __init__(self, bits: List[int]):
        self.bits = bits

    def __str__(self):
        return f"{self.bits}"

    def __or__(self, other):
        lst = []
        for item in range(len(self.bits)):
            items = self.bits[item] | other.bits[item]
            lst.append(items)
        return lst

    def __and__(self, other):
        lst = []
        for item in range(len(self.bits)):
            items = self.bits[item] & other.bits[item]
            lst.append(items)
        return lst

    def left_shift(self):
        """
        [1,0,1,0,1]
        [1,0,1,0,0]
        """
        lst = []
        for item in range(0, len(self.bits) - 1):
            lst.append(self.bits[item])
        lst.append(0)
        return lst

    def right_shift(self):
        lst = []
        lst.append(0)
        for item in range(1, len(self.bits)):
            lst.append(self.bits[item-1])
        return lst

    def extract(self, start: int, end: int):
        """
        [1, 0, 0, 1, 0, 1, 1, 1]
        bn.extract(2, 4) --> mask_length = 3)
        [0, 0, 0, 0, 0, 1, 0, 1]
        """
        mask_length = (end - start) + 1             # Number of 1's in our mask list
        padded_bits = len(self.bits) - mask_length  # Number of 0's in our mask list
        mask_list = []

        # Create our mask in list form, e.g. [1, 1, 1]

        # Add the 0's to pad the bitfield to the correct length
        for i in range(padded_bits):
            mask_list.append(0)

        # Add the 1's that make the mask
        for i in range(mask_length):
            mask_list.append(1)

        # Turn our mask into a BinaryNumber so we can use shift functions.
        # Remember, left_shift and right_shift are methods that are only
        # associated with the BinaryNumber class, so we need an extract_mask to
        # be a binary number to use them.
        extract_mask = BinaryNumber(mask_list)

        # To preserve the value of self.bits (not cause any side effects),
        # we copy our self.bits list.
        self_copy = BinaryNumber(self.bits.copy())

        # We shift our copied bits to the right to make our starting
        # bit accessible.
        # Since we only shift right by 1, we will repeat our shift
        # operation until we get to the starting bit.
        for i in range(start):
            self_copy.right_shift()

        # See the difference? We shifted right 2 since start = 2.
        # Now the 1's in our mask are aligned with our target bits.
        print(f"Unshifted: {self.bits}")
        print(f"Shifted: {self_copy.bits}")
        print(f"Mask: {extract_mask.bits}")

        # Extract the value by using & operator. We only keep our target bits
        # and discard the rest, e.g.
        #   1, 0, 0, 1, 0, 1, 1, 1
        # & 0, 0, 0, 0, 0, 1, 1, 1
        # = 0  0  0  0  0  1  1  1 <-- only keep bits 2, 3, 4
        return self_copy & extract_mask



"""bn = BinaryNumber([1,0,1,0,1])
bn2 = BinaryNumber([1,1,1,0,0])
print("1st binary number =",bn)
print("2nd binary number =", bn2)
print("AND",bn & bn2)
print("OR", bn | bn2)
bn.right_shift()
print("1st number right-shifted =", bn.right_shift())
bn.left_shift()
print("1st number left-shifted =", bn.left_shift())"""