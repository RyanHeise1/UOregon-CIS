"""Coding done in lab 1/20/21"""

from math import *

class Shape3D:
    """
    Creating main class

    __init__, volume, area all return not implemented erros becuase they wont be used here

    print_info prints the info about the shape that has a specific area and vilume
        Here, we can use class enhanentance to use that method
            ie: class Cylinder(Shape3D)
    """

    def __init__(self):
        #raise NotImplementedError("Abstract class cannot be instantiated")
        pass

    def volume(self) -> float:
        #raise NotImplementedError("Not implemented for abstract class")
        pass

    def area(self) -> float:
        #raise NotImplementedError("Not implemented for abstract class")
        pass

    def print_info(self):
        print(f"Area: {self.area()} Volume: {self.volume()}")

class Cylinder(Shape3D):
    """
    This is the class to find the area of a cylinder
    """
    def __init__(self, radius: float, height: float): # constructor method used to define variables that will be used for later use
        self.radius = radius # Defining the variables used above
        self.height = height

    def volume(self) -> float:
        """
        returns volume of the cylinder

        Uses the variables in the __init__ method and returns the volume
        """
        return pi * self.radius ** 2 * self.height

    def area(self) -> float:
        """
        returns area of the cylinder

        Uses the variables in the __init__ method and returns the area
        """
        return ((2 * pi * self.radius ** 2) + (2 * pi * self.radius * self.height))


class Cuboid(Shape3D):
    def __init__(self, width: float, length: float, height: float):
        self.width = width
        self.length = length
        self.height = height

    def volume(self) -> float:
        return self.width * self.length * self.height

    def area(self) -> float:
        return ((2 * self.width * self.length) + (2 * self.width * self.height) + (2 * self.length * self.height))


class Cube(Cuboid):
    def __init__(self, width: float):
        super().__init__(width, width, width)
        self.width = width

cyl = Cylinder(3,5)
cuboid = Cuboid(6,4,9)
lst = [ Cube(3), cyl, cuboid ]
for shape in lst:
    shape.print_info()