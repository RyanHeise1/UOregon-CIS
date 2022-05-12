"""
Code Demo 1/26/21 CIS 211
"""

class Complex:
    def __init__(self, real: float, imaginary: float):
        self.real = real
        self.imaginary = imaginary

    def __str__(self):
        return f"{self.real} + {self.imaginary}i"

    def __eq__(self, other):
        return self.real == other.real and self.imaginary == other.imaginary

    def __add__(self, other):
        new_real = self.real + other.real
        new_imagine = self.imaginary + other.imaginary
        return Complex(new_real, new_imagine)

    def absolute(self):
        return ((self.real) ** 2 + (self.imaginary) ** 2) ** (1/2)
