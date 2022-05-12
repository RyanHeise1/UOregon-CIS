"""Q1:  Building class"""

import unittest
from mini_exam import *

class Room:
    def __init__(self, building: str, room_num: int):
        self.building = building
        self.room_num = room_num

    def __str__(self):
        return f"{self.building} {self.room_num}"

    def same_building(self, other: "Room") -> bool:
        """These two rooms are in the same building"""
        return self.building == other.building



class Test_Room(unittest.TestCase):
    """Basic tests of the Room class"""

    def test_str(self):
        """Check the __str__ magic method"""
        deschutes243 = Room("Deschutes", 243)
        as_str = str(deschutes243)
        self.assertEqual(as_str, "Deschutes 243")

    def test_same_building(self):
        """Different rooms in the same building"""
        straub150 = Room("Straub", 150)
        straub254 = Room("Straub", 254)
        self.assertTrue(straub150.same_building(straub254))
        self.assertTrue(straub254.same_building(straub150))
        self.assertTrue(straub254.same_building(straub254))

    def test_diff_building(self):
        """Rooms in different buildings"""
        deschutes243 = Room("Deschutes", 243)
        straub150 = Room("Straub", 150)
        self.assertFalse(straub150.same_building(deschutes243))
        self.assertFalse(deschutes243.same_building(straub150))




if __name__ == "__main__":
    unittest.main()




