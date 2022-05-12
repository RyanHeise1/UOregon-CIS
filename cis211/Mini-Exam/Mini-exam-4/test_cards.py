"""Test cases for cards.py"""

import unittest
from cards import *

class TestCardEncode(unittest.TestCase):

    def test_encode(self):
        # Expected values of the encode function
        card = encode(CardRank.Jack, CardSuit.Hearts)
        self.assertEqual(card, 11)
        card = encode(CardRank.Ace, CardSuit.Spades)
        self.assertEqual(card, 33)
        card = encode(CardRank.Four, CardSuit.Diamonds)
        self.assertEqual(card, 20)
        card = encode(CardRank.King, CardSuit.Clubs)
        self.assertEqual(card, 61)

    def test_decode(self):
        rank, suit = decode(11)
        self.assertEqual(rank, CardRank.Jack)
        self.assertEqual(suit, CardSuit.Hearts)
        rank, suit = decode(33)
        self.assertEqual(suit, CardSuit.Spades)
        self.assertEqual(rank, CardRank.Ace)
        rank, suit = decode(20)
        self.assertEqual(rank, CardRank.Four)
        self.assertEqual(suit, CardSuit.Diamonds)
        rank, suit = decode(61)
        self.assertEqual(rank, CardRank.King)
        self.assertEqual(suit, CardSuit.Clubs)

class TestHandPack(unittest.TestCase):
    """Packing and unpacking a whole hand consisting
    of five Poker cards.
    """
    def setUp(self) -> None:
        self.hand = [(CardRank.King, CardSuit.Diamonds),
                    (CardRank.Queen, CardSuit.Hearts),
                    (CardRank.Jack, CardSuit.Clubs),
                    (CardRank.Ten, CardSuit.Spades),
                    (CardRank.Ace, CardSuit.Diamonds)]

    def test_fits_in_30(self):
        self.assertLess(pack_hand(self.hand), 1 << 30)

    def test_is_inverse(self):
        packed = pack_hand(self.hand)
        unpacked = unpack_hand(packed)
        self.assertEqual(unpacked, self.hand)


if __name__ == "__main__":
    unittest.main()
