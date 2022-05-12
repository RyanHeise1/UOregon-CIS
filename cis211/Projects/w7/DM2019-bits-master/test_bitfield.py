"""Unit tests for bitfield.py"""

from bitfield import BitField
import unittest


def sign_extend(field: int, width: int) -> int:
    """Interpret field as a signed integer with width bits.
    If the sign bit is zero, it is positive.  If the sign bit
    is negative, the result is sign-extended to be a negative
    integer in Python.
    width must be 2 or greater. field must fit in width bits.
    # Examples:
    Suppose we have a 3 bit field, and the field
    value is 0b111 (7 decimal).  Since the high
    bit is 1, we should interpret it as
    -2^2 + 2^1  + 2^0, or -4 + 3 = -1

    Suppose we have the same value, decimal 7 or
    0b0111, but now it's in a 4 bit field.  In thata
    case we should interpret it as 2^2 + 2^1 + 2^0,
    or 4 + 2 + 1 = 7, a positive number.
    """
    assert width > 1
    assert field >= 0 and field < 1 << (width + 1)
    sign_bit = 1 << (width - 1)  # will have form 1000... for width of field
    mask = sign_bit - 1  # will have form 0111... for width of field
    if (field & sign_bit):
        # It's negative; sign extend it
        extended = (field & mask) - sign_bit
        return extended
    else:
        return field


class Test_Extract(unittest.TestCase):

    def test_extract_low(self):
        """Extract low 3 bits"""
        low_bits = BitField(0,2)
        self.assertEqual(low_bits.extract(0b10101010101), 0b101)

    def test_middle_bits(self):
        """Extract 5 bits from the middle of a word"""
        middle_bits = BitField(5,9)
        self.assertEqual(middle_bits.extract(0b1010101101101011), 0b11011)

    def test_insert_low(self):
        """Inserting a few bits in the lowest part of the word. """
        low_bits = BitField(0, 3)
        self.assertEqual(low_bits.insert(15, 0), 15)  # All the bits to 1
        # Slip it in without disturbing higher bits
        self.assertEqual(low_bits.insert(0b1010, 0b1111_0000), 0b1111_1010)


class Test_Sign_Extension(unittest.TestCase):

    def test_extend_positive(self):
        """Sign extension of a positive number doesn't change it.  Note high
        bit in field must be zero.  7 is a positive number in a 3-bit field,
        but a (different) negative number in a 3-bit field.
        """
        self.assertEqual(sign_extend(7,4), 7)
        self.assertNotEqual(sign_extend(7,3), 7)
        self.assertTrue(sign_extend(7,3) < 0)

    def test_extend_negative(self):
        """For negative numbers, sign extension restores the high bits"""
        chunk = (-3) & 0b111
        self.assertEqual(sign_extend(chunk,3), -3)


class Test_Signed_Extraction(unittest.TestCase):

    def test_extract_neg(self):
        bitfield = BitField(2,4)
        field_bits = 0b_101_111_10  # the 111 part is what we want to extract
        self.assertEqual(bitfield.extract_signed(field_bits), -1)

    def test_extract_pos(self):
        bitfield = BitField(2,4)
        field_bits = 0b_101_011_10  # the 011 part is what we want to extract
        self.assertEqual(bitfield.extract_signed(field_bits), 3)


class Test_Signed_Insert(unittest.TestCase):

    def test_insert_neg(self):
        bitfield = BitField(3,5)
        packed = bitfield.insert(-1, 0)
        self.assertEqual(packed, 0b000_111_000)
        unpacked = bitfield.extract_signed(packed)
        self.assertEqual(unpacked, -1)


class Insert_Value(unittest.TestCase):
    def test_insert(self):
        bitfield = BitField(0, 3)
        packed = bitfield.insert(1, 2)
        self.assertEqual(packed, 0b11)

        bitfield = BitField(4, 6)
        packed = bitfield.insert(1, 2)
        self.assertEqual(packed, 0b10010)

        bitfield = BitField(7, 10)
        packed = bitfield.insert(1, 2)
        self.assertEqual(packed, 0b10000010)


class Extract_Value(unittest.TestCase):
    def test_extract(self):
        bitfield = BitField(0, 3)
        packed = bitfield.insert(1, 2)
        self.assertEqual(packed, 0b11)
        unpacked = bitfield.extract_signed(packed)
        self.assertEqual(unpacked, 3)

        bitfield = BitField(4, 6)
        packed = bitfield.insert(1, 2)
        self.assertEqual(packed, 0b10010)
        unpacked = bitfield.extract_signed(packed)
        self.assertEqual(unpacked, 1)

        bitfield = BitField(7, 10)
        packed = bitfield.insert(1, 2)
        self.assertEqual(packed, 130)
        unpacked = bitfield.extract_signed(packed)
        self.assertEqual(unpacked, 1)

"""if we have a field that is made up of bits 0..3, another 
bit field 4..6, and another 7..10, inserting and extracting values from the middle 
field should neither affect values in the first and third field nor be affected by them"""


if __name__ == "__main__":
    unittest.main()