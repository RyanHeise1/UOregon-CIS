"""Live coding done in Lab 1/13/21"""

"""

"""
import random

class Pokemon:
    def __init__(self, p_name: str, p_type: str, p_gender: str, p_moveset: list = []):
        assert p_gender == "f" or p_gender == "m" or p_gender == "u"
        self.name = p_name
        self.type = p_type
        self.sex = p_gender
        self.moveset = p_moveset

    def __str__(self):
        return f"Name: {self.name} \n Type: {self.type} \n Sex: {self.sex} \n Moveset: {self.moveset}"

    def forget_move(self, move_index: int) -> None:
        assert len(self.moveset) == 4 and 0 <= move_index <= 3 #pokeon can only have 4 mmoves at a time
        self.moveset.pop(move_index)

    def learn_move(self, move_name: str, remove_index: int) -> None:
        self.forget_move(remove_index)
        self.moveset.append(move_name)

    def breed(self, other: "Pokemon", new_name: str) -> "Pokemon":
        assert self.sex != other.sex
        if random.random() >= .5:
            new_type = self.type
            new_sex = self.sex
        else:
            new_type = other.type
            new_sex = other.sex

        return Pokemon(new_name, new_type, new_sex, self.moveset[0:2] + other.moveset[2:])


p1 = Pokemon("Pikachu", "electric", "f", ["thunderbolt", "tackle", "quick attack", "iron tail"])
p2 = Pokemon("Charmander", "electric", "m", ["ember", "scratch", "glame thrower", "growl"])

"""
print(p1)
print()
print(p2)
print()
print()

p3 = p1.breed(p2, "Pikachar")
print(p3)
"""