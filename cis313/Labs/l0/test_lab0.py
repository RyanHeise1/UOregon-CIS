import lab0
import unittest


class T0_TestingGCD(unittest.TestCase):
    
    def test_testing_with_zero(self):
        #testing with zero
        g1 = lab0.mathOps(0, 0)
        print(g1)
        print(repr(g1))
        
        with self.assertRaises(Exception):
            g1.gcd()
            g1.lcm()

        print("\n")

class T1_TestingGCD(unittest.TestCase):
    
    def test_testing_with_float(self):
        #testing with float
        g1 = lab0.mathOps(39.7, 7.9)
        print(g1)
        print(repr(g1))
        self.assertEqual(8, g1.gcd())
        self.assertEqual(40, g1.lcm())
        # with self.assertRaises(Exception):
        #     g1.lcm()
        print("\n")

class T2_TestingGCD(unittest.TestCase):
    
   def test_testing_with_negative(self):
       #testing with negative numbers
       g1 = lab0.mathOps(-6, 9)
       print(g1)
       self.assertEqual(g1.gcd(), 3)
       print("\n")

class T3_TestingGCD(unittest.TestCase):
   
   def test_testing_with_negative_and_zero(self):
       # testing with zero and negative numbers
       g1 = lab0.mathOps(-6, 0)
       print(g1)
       self.assertEqual(g1.gcd(), 6)
       print("\n")

class T4_TestingGCD(unittest.TestCase):
   
   def test_testing_with_string(self):
       # testing with strings
       g1 = lab0.mathOps("hello", "world")
       print(g1)
       print(repr(g1))
       with self.assertRaises(Exception):
           g1.gcd()
           g1.lcm()
       print("\n")

class T5_TestingGCD(unittest.TestCase):
   
   def test_testing_with_infinity(self):
       # testing with infinity
       g1 = lab0.mathOps(0 , float("inf"))
       print(g1)
       print(repr(g1))
       with self.assertRaises(Exception):
           g1.gcd()
           g1.lcm()
       print("\n")

class T6_TestingGCD(unittest.TestCase):
   
   def test_testing_with_powers(self):
       # testing with large numbers
       g1 = lab0.mathOps(2 ** 10, 2 ** 11)
       print(g1)
       print(repr(g1))
       self.assertEqual(g1.gcd(), 1024)
       self.assertEqual(g1.lcm(), 2048)
       print("\n")

class T7_TestingGCD(unittest.TestCase):
   
   def test_testing_with_powers(self):
       #testing with large numbers
       g1 = lab0.mathOps(2 ** 29, 2 ** 28)
       print(g1)
       print(repr(g1))
       self.assertEqual(g1.gcd(), 268435456)
       self.assertEqual(g1.lcm(), 536870912)
       print("\n")

class T8_TestingGCD(unittest.TestCase):
   
   def test_testing_with_largepowers(self):
       # testing with large numbers
       g1 = lab0.mathOps(2 ** 62, 2 ** 50 )
       print(g1)
       print(repr(g1))
       self.assertEqual(g1.gcd(), 1125899906842624)
       self.assertEqual(g1.lcm(), 4.611686018427388e+18)
       print("\n")



class T10_TestingGCD(unittest.TestCase):
   
   def test_testing_with_invalid_ints(self):
       # testing with large numbers
       g1 = lab0.mathOps(2 ** 100, 2 ** 70 )
       print(g1)
       print(repr(g1))
       self.assertEqual(g1.gcd(), 2 ** 70)
       self.assertEqual(g1.lcm(), 2 ** 100)
       print("\n")

class T11_TestingGCD(unittest.TestCase):
   
   def test_testing_with_prime_ints(self):
       # testing with prime numbers
       g1 = lab0.mathOps(29, 31 )
       print(g1)
       print(repr(g1))
       self.assertEqual(g1.gcd(), 1)
       self.assertEqual(g1.lcm(), 899)
       print("\n")

class T12_TestingGCD(unittest.TestCase):
   
   def test_testing_with_infinity(self):
       # testing with infinity
       g1 = lab0.mathOps( float("inf"), float("inf"))
       print(g1)
       print(repr(g1))
       with self.assertRaises(Exception):
           g1.gcd()
           g1.lcm()
       print("\n")


if __name__ == '__main__':
    unittest.main()
