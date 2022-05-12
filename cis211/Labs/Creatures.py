"""
Live coding done in class 3/3/21
Study for final
"""


class Creature(object):
    def __init__(self):
        raise NotImplementedError("Abstract classes should not be instanciated")

    def __str__(self) -> str:
        raise NotImplementedError("Abstract class methods should not be called")

    def search(self, value: str) -> bool:
        raise NotImplementedError("Abstract class methods should not be called")


class Head(Creature): # 1 head (middle)
    def __init__(self, middle):
        self.middle = middle

    def __str__(self) -> str:
        return f"{str(self.middle)}"

    def search(self, value: str) -> bool:
        if self.middle == value:
            return True
        return False


class Orthrus(Creature): # 2 heads (left, right)
    def __init__(self, right, left):
        self.right = right
        self.left = left

    def __str__(self) -> str:
        return f"{str(self.right)} {str(self.left)}"

    def search(self, value: str) -> bool:
        return self.right.search(value) == True or self.left.search(value) == True


class Cerberus(Creature): # 3 heads (left, middle, right)
    def __init__(self, right, middle, left):
        self.right = right
        self.middle = middle
        self.left = left

    def __str__(self) -> str:
        return f"{str(self.right)} {str(self.middle)} {str(self.left)}"

    def search(self, value: str) -> bool:
        # return self.right.search(value) == True or self.middle.search(value) == True or self.left.search(value) == True
        return self.right.search(value) or self.middle.search(value) or self.left.search(value) # Same thing as above


doggy1 = Head("Kane")
doggy2 = Head("Wolfie")
doggy3 = Head("Rugal")
doggy4 = Head("Taker")
ort1 = Orthrus(doggy1, doggy2)
ort2 = Orthrus(doggy3, Head("Jeff"))
cer = Cerberus(ort1, doggy4, ort2)

print(cer)
print(cer.search("Drogon"))
print(cer.search("Rugal"))
