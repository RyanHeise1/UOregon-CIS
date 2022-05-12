"""Test cases for food.py"""

from typing import List
import unittest
from food import *

def foodsort(foods: List[Food]) -> List[Food]:
    return sorted(foods, key=str)

class TestBasics(unittest.TestCase):

    def test_just_water(self):
        """Water contains no allergens (we hope)"""
        water = Basic("water")
        vegan = [Irritant.MEAT, Irritant.MILK]
        self.assertEqual(water.bad_ingredients(vegan), [])

    def test_omnivores_can_drink_water(self):
        avoiding = []
        water = Basic("water")
        self.assertEqual(water.bad_ingredients(avoiding), [])

    def test_lactose_intolerant(self):
        lactose = [Irritant.MILK]
        ice_cream = Basic("ice_cream", irritants=lactose)
        self.assertEqual(ice_cream.bad_ingredients(lactose), [ice_cream])

    def test_omnivores_drink_milk(self):
        avoiding = []
        lactose = [Irritant.MILK]
        ice_cream = Basic("ice_cream", irritants=lactose)
        self.assertEqual(ice_cream.bad_ingredients(avoiding), [])

    def test_vegans_dont_drink_milk(self):
        avoiding = [Irritant.MEAT, Irritant.MILK]
        ice_cream = Basic("ice_cream", irritants=[Irritant.MILK])
        self.assertEqual(ice_cream.bad_ingredients(avoiding), [ice_cream])


class TestComposites(unittest.TestCase):

    def test_simple_harmless_composite(self):
        avoiding = [Irritant.MEAT, Irritant.MILK]
        sauce = Composite("sauce", [Basic("tomato"), Basic("spices")])
        sauce_bad = sauce.bad_ingredients(avoiding)
        self.assertEqual(sauce_bad, [])

    def test_omnivores_eat_sauce(self):
        avoiding = []
        sauce = Composite("sauce", [Basic("tomato"), Basic("spices")])
        sauce_bad = sauce.bad_ingredients(avoiding)
        self.assertEqual(sauce_bad, [])

    def test_gluten_free(self):
        avoiding = [Irritant.GLUTEN]
        flour = Basic("flour", irritants=[Irritant.GLUTEN])
        water = Basic("water")
        yeast = Basic("yeast")
        salt = Basic("salt")
        dough = Composite("dough", [water, flour, yeast, salt])
        dough_bad = dough.bad_ingredients(avoiding)
        self.assertEqual(dough_bad, [flour])


class TestPizza(unittest.TestCase):
    #Composite of composites, multiple potentially troublesome ingredients (but yummy!)


    def setUp(self) -> None:
        self.flour = Basic("flour", irritants=[Irritant.GLUTEN])
        self.dough = Composite("dough",
                               [self.flour, Basic("yeast"), Basic("water")])
        self.salami = Basic("salami", irritants=[Irritant.MEAT])
        self.cheese = Basic("cheese", irritants=[Irritant.MILK])
        self.topping = Composite("topping", [self.cheese, self.salami])
        self.sauce = Composite("sauce", [Basic("tomato"), Basic("spices")])
        self.pizza = Composite("pizza", [self.dough, self.sauce, self.topping])

    def test_omnivores_eat_pizza(self):
        avoid = []
        pizza_bad = self.pizza.bad_ingredients(avoid)
        self.assertEqual(pizza_bad, [])

    def test_i_ordered_vegan(self):
        avoid = [Irritant.MILK, Irritant.MEAT]
        pizza_bad = foodsort(self.pizza.bad_ingredients(avoid))
        self.assertEqual(pizza_bad,
                         foodsort([self.cheese, self.salami]))

    def test_gluten_intolerant(self):
        avoid = [Irritant.GLUTEN]
        pizza_bad = foodsort(self.pizza.bad_ingredients(avoid))
        self.assertEqual(pizza_bad, [self.flour])

    def test_no_sugar(self):
        # It could be worse!
        avoid = [Irritant.SUGAR_ADD]
        pizza_bad = self.pizza.bad_ingredients(avoid)
        self.assertEqual(pizza_bad, [])

if __name__ == "__main__":
    unittest.main()







