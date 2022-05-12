"""Mini-exam 1, week 2, CIS 211 Winter 2021
University of Oregon

Your name: Ryan Heise
"""
from typing import List

# See the HOWTO for what to place below.

## Q1 asks you to build a class called Room ...
class Room:
    def __init__(self, building: str, room_num: int):
        self.building = building
        self.room_num = room_num

    def __str__(self):
        return f"{self.building} {self.room_num}"

    def same_building(self, other: "Room") -> bool:
        """These two rooms are in the same building"""
        return self.building == other.building


## Q2 asks you to build a function
def outdoor_passing(classes: List[Room]) -> int:
    ct = 0
    if len(classes) == 0:
        return 0
    if len(classes) == 1:
        return 0
    for item in range(len(classes)-1):
        if Room.same_building(classes[item], classes[item + 1]):
            pass
        else:
            ct += 1
    return ct

