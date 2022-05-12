"""
Project: BitField
Name: Ryan Heise

A bit field is a range of binary digits within an
unsigned integer.   Bit 0 is the low-order bit,
with value 1 = 2^0.  Bit 31 is the high-order bit,
with value 2^31.

A bitfield object is an aid to encoding and decoding
instructions by packing and unpacking parts of the
instruction in different fields within individual
instruction words.
"""

import logging
logging.basicConfig()
log = logging.getLogger(__name__)
log.setLevel(logging.DEBUG)

# Size of the DM2019 which is 32 bits
WORD_SIZE = 32


class BitField(object):
    """A BitField object extracts specified
    bitfields from an integer.
    """
    def __init__(self, from_bit: int, to_bit: int) -> None:
        """Tool for  extracting bits
        from_bit ... to_bit, where 0 is the low-order
        bit and 31 is the high-order bit of an unsigned
        32-bit integer. For example, the low-order 4 bits
        could be represented by from_bit=0, to_bit=3.
        """
        # Make sure that the start and end of the bit is in range of the max bit length
        assert 0 <= from_bit < WORD_SIZE
        assert from_bit <= to_bit <= WORD_SIZE
        self.from_bit = from_bit
        self.to_bit = to_bit

        # Calc Field Width
        self.width = (to_bit - from_bit) + 1

        # Mask For Low-Order Bits
        self.lower_mask = self.mask(self.width)

        # Sign extension complement and mask
        self.complement = 2 ** self.width
        self.sign_bit = 2 ** (self.width - 1)

    def mask(self, width: int) -> int:
        """
        Makes a mask based on the width of the bit
        Ex:
        mask(5)
        returns --> 0b11111
        """
        # Make sure we dont have a negative number
        assert width >= 0
        # variable to store the bit based on the length
        mask = 0
        # Looping through and making a mask based on how long the width is
        for bit in range(width):
            mask = (mask << 1) | 1
        return mask

    def extract(self, word: int) -> int:
        """Extract the bitfield and return it in the
        low-order bits.  For example, if we are extracting
        bits 3..5, the result will be an
        integer between 0 and 7 (0b000 to 0b111).
        """
        # shift bit to the right based on the from_bit
        ext = word >> self.from_bit
        # returning the result of the shifted word & the mask
        return ext & self.lower_mask

    def insert(self, value: int, word: int) -> int:
        """Insert value, which should be in the low order
         bits and no larger than the bitfield, into the
         bitfield, which should be zero before insertion.
         Returns the combined value.
         Example: BitField(3,5).insert(0b101, 0b110) == 0b101110
         """
        ins = (value & self.lower_mask) << self.from_bit
        return ins | word

    def extract_signed(self, word: int) -> int:
        """Extract bits in bitfield as a signed integer."""
        unsigned = self.extract(word)
        if unsigned & self.sign_bit:
            return 0 - (self.complement - unsigned)
        else:
            return unsigned
