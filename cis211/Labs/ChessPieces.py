class ChessPieces: # concrete class
    def __init__(self):
        raise NotImplementedError("Abstract classes is not supposed to be instantiated")

    def move(self):
        raise NotImplementedError("Method not implemented for abstact class")

    def __str__(self):
        return f"{self.color} {self.name} located at {self.x_pos}, {self.y_pos}"

class Rook(ChessPieces): # concrete class
    def __init__(self, clr: str, x: int, y: int) -> "Rook":
        self.name = "rook"
        self.color = clr
        self.x_pos = x
        self.y_pos = y

    def __str__(self):
        return super().__str__() + " I'm a rook"

    def move(self):
        print("I'm a rook I move in a straight line")


class Bishop(ChessPieces): # concrete class
    def __init__(self, clr: str, x: int, y: int) -> "Rook":
        self.name = "Bishop"
        self.color = clr
        self.x_pos = x
        self.y_pos = y

    def __str__(self):
        return super().__str__() + " I'm a Bishop"

    def move(self):
        print("I'm a Bishop I move in a diagonal line")

class Knight(ChessPieces): # concrete class
    def __init__(self, clr: str, x: int, y: int) -> "Rook":
        self.name = "Knight"
        self.color = clr
        self.x_pos = x
        self.y_pos = y

    def __str__(self):
        return super().__str__() + " I'm a Knight"

    def move(self):
        print("I'm a Knight I move in a L shape")

r1 = Rook("black", 2, 5)
b1 = Bishop("white", 1, 3)
print(r1)
chess_pieces = [r1, b1, Knight("white", 6, 7)]

for piece in chess_pieces:
    piece.move()