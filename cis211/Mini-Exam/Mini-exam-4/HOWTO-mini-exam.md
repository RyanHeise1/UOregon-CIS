# Week 8 mini-exam HOWTO

This week's mini-exam asks you
to encode playing card suit
and rank as integers, and to 
decode them. 

## Background: Playing cards

### The cards 
There are many kinds of playing
cards, for many card games.  For this
problem we consider the cards used
in the game Poker and some related
games.

Each poker card has a suit and a
rank.  The suits are called
Hearts, Diamonds, Spades, and Clubs. 
The ranks are 1-10, Jack, Queen,
and King.  The 1 card is called an
Ace, so the only cards numbered 2-10
are actually named for a number.

Since there are four suits, and 13
ranks in each suit, there are 52
cards in a "deck" of poker cards. 

We describe a card by its rank and suit.
For example, "Six of Spades" is a card
with rank 6 and suit "Spades".  

### A "hand" of cards

In poker, a "hand" is a collection of 5 
poker cards.  

## Q1: Encoding and Decoding Cards

In file `cards.py`, the suits and ranks
of cards are represented 
by enumerations `CardSuit` and 
`CardRank`.  

We will encode the rank and suit of a card together
in a single integer, using bits 0..3 to represent
its rank and bits 4..5 to represent its suit. You will
write two functions: 

`encode` takes a `CardRank` and a `CardSuit` and 
returns an integer representing both, using 4 bits 
(bits 0..3) to represent the `CardRank` and 2 bits
(bits 4..5) to represent the `CardSuit`.  Its signature
is 
```python
def encode(rank: CardRank, suit: CardSuit) -> int:
```
`decode` takes an integer that has been produced by 
`encode`.  It returns the `CardRank` and `CardSuit`
that were packed into the integer.  (You may assume
the input integer was produced by `encode`.)  The signature
of `decode` is 
```python
def decode(card: int) -> Tuple[CardRank, CardSuit]:
```

## Q2: Encoding a "hand" of five cards

Since the integer encoding a single card fits in
6 bits (4 bits for the rank and 2 for the suit), 
a whole "hand" consisting of five cards can be
packed into the low 30 bits of a 32-bit integer. 
This is what `pack_hand` should do. It's 
signature is 
```python
def pack_hand(cards: List[Tuple[CardRank, CardSuit]]) -> int:
```

`unpack_hand` performs the inverse of `pack_hand`:  It takes 
a single integer (which we will assume was produced by
`pack_hand`) and returns a list of five (`CardRank`, `CardSuit`)
tuples.  

```python
def unpack_hand(packed: int) -> List[Tuple[CardRank, CardSuit]]:
```

The main function demonstrates how `pack_hand` and `unpack_hand`
can be called. 

### Hints

It is easiest to write `pack_hand` as a loop.
On each iteration of the loop, encode one
card as an integer and 'or' it into the low-order
bits of the result.  Shift the result to make
room before each iteration. 

`unpack_hand` can similarly be written as a loop
that iterates exactly five times (one for each card
in a standard Poker hand).  On each loop, we can
*mask off* six bits and then use the `decode` 
function to convert those six bits into a (rank, suit)
pair.  

There is one little trick:  If you write `pack_hand`
and `unpack_hand` as suggested above, the result
of `unpack_hand(pack_hand(hand))` will be in the
reverse order of `hand`.  The easiest way to fix this
is simply to reverse the result list in `unpack_hand`, 
after building it and before returning it. 


## Simple test

I have provided a small
"smoke test" in the main program.  If the `encode` and `decode`
functions are correct, the output of `main` should be

```
Jack of Hearts => 11 => Jack of Hearts
Ace of Spades => 33 => Ace of Spades
Four of Diamonds => 20 => Four of Diamonds
King of Clubs => 61 => King of Clubs
Hand:
King of Diamonds
Queen of Hearts
Jack of Clubs
Ten of Spades
Ace of Diamonds
```
These are essentially the same test cases in 
`test_cards.py`.

