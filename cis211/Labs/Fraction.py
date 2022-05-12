"""Lab 1/13/21"""

class Fraction:
    def __init__(self, num: int, den: int):
        assert (num >= 0 and (den > 0)), f"Denominator cannot be 0 and Numerator cannot be negative"
        self.num = num
        self.den = den

    def __str__(self):
        return f"{self.num}/{self.den}"

    def __repr__(self):
        return f"Fraction({self.num},{self.den})"

    def __mul__(self, other: "Fraction") -> 'Fraction':
        new_num = self.num * other.num
        new_den = self.den * other.den

        return Fraction(new_num, new_den)

    def __add__(self, other: "Fraction") -> "Fraction":
        new_num = self.num * other.den + self.den * other.num
        new_den = self.den * other.den

        return Fraction(new_num, new_den)

    def gcd(self, a, b) -> int:
        while a != b:
            if a > b:
                a -= b
            else:
                b -= a
        return a

    def simplify(self):
        factor = self.gcd(self.num, self.den)
        self.num = int(self.num / factor)
        self.den = int(self.den / factor)





"""f1 = Fraction(6,8)
f2 = Fraction(4,0)"""