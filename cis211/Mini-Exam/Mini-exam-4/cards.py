"""Playing cards (Poker deck) encoded
as one-byte integers
"""

import enum
from typing import Tuple, List
# Not sure if we are able to use bitfield, but I thought of a better way to do pakc_hand
from bitfield import *

class CardSuit(enum.Enum):
    value: int      # Hint to typechecker
    Hearts = 0
    Diamonds = 1
    Spades = 2
    Clubs = 3

class CardRank(enum.Enum):
    value: int      # Hint to typechecker
    Ace = 1
    Two = 2
    Three = 3
    Four = 4
    Five = 5
    Six = 6
    Seven = 7
    Eight = 8
    Nine = 9
    Ten = 10
    Jack = 11
    Queen = 12
    King = 13

# Note we need 6 bits total for a card
#   Bits 0..3 for CardRank (4 bits)
#   Bits 4..5 for CardSuit (2 bits)

def encode(rank: CardRank, suit: CardSuit) -> int:
    """
    CardRank and a CardSuit and returns an integer representing both,
    using:
    4 bits (bits 0..3) to represent the CardRank
    2 bits (bits 4..5) to represent the CardSuit
    """
    suit_val = suit.value << 4
    rank_val = rank.value
    return rank_val | suit_val

    # return int(bin(suit_val) + format(rank_val, "04b"), 2)


def decode(card: int) -> Tuple[CardRank, CardSuit]:
    """
    takes an integer that has been produced by encode

    returns the CardRank and CardSuit that were packed into the integer
    """
    card_rank = card & 0b1111
    card_suit = card >> 4

    return CardRank(card_rank), CardSuit(card_suit)

def desc(card: int):
    rank, suit = decode(card)
    return f"{rank.name} of {suit.name}"


def pack_hand(cards: List[Tuple[CardRank, CardSuit]]) -> int:
    """Pack a list of 5 (rank, suit) pairs into
    30 bits of an integer.
    """
    assert len(cards) == 5, "Poker hand is 5 cards"
    hand = 0

    for rank, suit in cards:
        card = encode(rank,suit)
        hand = (hand << 6) | card

    return hand



def unpack_hand(packed: int) -> List[Tuple[CardRank, CardSuit]]:
    """Inverse of pack_hand: Unpack a single integer
    into a list of 5 (CardRank, CardSuit) pairs.
    """
    hand = []

    for i in range(5):
        card_i = packed & 0b111111
        card = decode(card_i)
        hand.append(card)
        packed >>= 6
    hand.reverse()
    return hand

def main():
    """Smoke test"""
    card = encode(CardRank.Jack, CardSuit.Hearts)
    print(f"Jack of Hearts => {card} => {desc(card)}")
    card = encode(CardRank.Ace, CardSuit.Spades)
    print(f"Ace of Spades => {card} => {desc(card)}")
    card = encode(CardRank.Four, CardSuit.Diamonds)
    print(f"Four of Diamonds => {card} => {desc(card)}")
    card = encode(CardRank.King, CardSuit.Clubs)
    print(f"King of Clubs => {card} => {desc(card)}")

    # Expected output:
    # Jack of Hearts => 11 => Jack of Hearts
    # Ace of Spades => 33 => Ace of Spades
    # Four of Diamonds => 20 => Four of Diamonds
    # King of Clubs => 61 => King of Clubs

    hand = [(CardRank.King, CardSuit.Diamonds),
            (CardRank.Queen, CardSuit.Hearts),
            (CardRank.Jack, CardSuit.Clubs),
            (CardRank.Ten, CardSuit.Spades),
            (CardRank.Ace, CardSuit.Diamonds)]
    packed = pack_hand(hand)
    # Result should be an integer, in 30 bits.
    # It should therefore be less than 2^30.
    assert packed < (1 << 30), "Should fit in 30 bits"
    unpacked = unpack_hand(packed)
    assert unpacked == hand, "Should be the same"
    print("Hand:")
    for rank, suit in unpacked:
        print(f"{rank.name} of {suit.name}")


if __name__ == "__main__":
    main()
