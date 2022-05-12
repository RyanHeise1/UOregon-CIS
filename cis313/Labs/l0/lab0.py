import math
class mathOps:

    def __init__(self, u, v):
        self.u = u
        self.v = v
    
    def __repr__(self):
        return "LeastCommonMultiple({}, {})".format(self.u, self.v)
    
    def __str__(self):
        return "GreatestCommonDivisor({}, {}).".format(self.u, self.v)

    def valid(self):
        return isinstance(self.u, int) and isinstance(self.v, int)
    
    def gcd(self):
      # Find the greatest common divisor of u and v
      # Hint: Use Euclid's Algorithm
      # https://en.wikipedia.org/wiki/Euclidean_algorithm#Procedure
        tempU = self.u
        tempV = self.v
        try:
          if tempU == (float("inf") or float("-inf")) or tempV == (float("inf") or float("-inf")):
            raise OverflowError
          
          tempU = mathOps.ceiling(tempU)
          tempV = mathOps.ceiling(tempV)
          if tempV < 0:
            tempV = tempV * -1
          if tempU < 0:
            tempU = tempU * -1
          if tempV == 0 or tempU == 0:
            return max(tempU, tempV)

          # ENTER YOUR CODE HERE 
          # Feel free to modify the exceptions, delete the try block etc or the entire funtion
          # Just keep the fucntion name as gcd
          # print(f'{tempV}, {tempU}')
          while (tempU % tempV > 0):
            remainder = tempU % tempV
            tempU = tempV
            tempV = remainder
          return tempV

        except OverflowError:
          print("one or both the values of", tempU, " and ", tempV, "are equal to infinity")
          raise OverflowError      

    def ceiling(n):
      if isinstance(n, float):
        return int(n) + 1
      return n

        
    def lcm(self):
      # Find the least common multiple of a and b
      # Hint: Use the gcd of a and b
      try:
        tempU = self.u
        tempV = self.v
        if tempU == (float("inf") or float("-inf")) or tempV == (float("inf") or float("-inf")):
            raise OverflowError
        tempU = mathOps.ceiling(tempU)
        tempV = mathOps.ceiling(tempV)
        # ENTER YOUR CODE HERE
        # Feel free to modify the exceptions, delete the try block etc or the entire funtion
        # Just keep the fucntion name as lcm
        
        if tempU > tempV:
            greater = tempU
        else:
            greater = tempV
        while True:
            if ((greater % tempU == 0) and (greater % tempV == 0)):
                lcm = greater
                break
            greater += 1
        return lcm
      except OverflowError:
          print("one or both the values of", tempU, " and ", tempV, "are equal to infinity")
          raise OverflowError
