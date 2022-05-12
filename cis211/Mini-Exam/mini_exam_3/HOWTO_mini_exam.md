# Mini-exam 3 (week 6)

## Recursive foods

Many foods are made up of ingredients that
are also foods.  We can think of a complex
food like a pizza as the root of 
a tree of ingredients,
with some basic ingredients like flour,
salt, and tomatoes at the leaves. 

In `food.py`, we have an abstract base 
class `Food` and two concrete subclasses,
`Basic`  (leaves of the tree) and 
`Composite` (foods that are composed of
other foods).  For example, we 
might define flour as a `Basic` food
(not breaking it down farther) and pizza
as a `Composite` food (describing it as 
a combination of crust, sauce, and toppings. 
The ingredients of a `Composite` food might 
be a `Basic` food like flour, or might itself
a `Composite` food.  For example, the toppings
on a pizza might be a combination of 
salami and cheese.  

## Irritants

Many people avoid some foods, for medical
reasons (e.g., they may be lactose intolerant), 
or for other reasons (e.g., they might be vegetarian
or vegan).  We can classify foods as containing
potential irritants, which could be anything
that some people avoid.  

I have provided an enumeration `Irritant`
with members like `Irritant.MILK`
and `Irritant.SOY`.  

When we create a `Basic` food like flour,
we indicate which irritants (if any) it
contains. 

```python
class Basic(Food):
    def __init__(self, name: str, irritants: List[Irritant] = []):
```

Since the empty list is a default value for
parameter `irritants`, we can create some
basic foods without specifying any irritants 
explicitly: 

```python
water = Basic("water")
```
Other basic foods may contain one or more
potential irritants: 

```python
cheese = Basic("cheese", irritants=[Irritant.MILK])
```

## Bad ingredients

Does this pizza contain bad ingredients?
That depends on what you want to avoid. 
Consider a pizza topped with cheese and salami. 
If you are vegan, then you will likely avoid
the cheese (milk) and salami (meat).  If you
are gluten intolerant, you may not mind the
cheese but avoid the crust, which contains flour. 

We therefore define a method in 
class `Food` for determining
which ingredients you may be avoiding.  One of
the parameters for this method will be
a list of `Irritant` that you wish to
identify in any part of the food. Note
that `bad_ingredients` returns a list
of basic foods like flour, not a list
of the irritants in those foods. 

For example, consider a test case that
should identify the flour in dough as a
concern for people who are gluten
intolerant: 

```python
    def test_gluten_free(self):
        avoiding = [Irritant.GLUTEN]
        flour = Basic("flour", irritants=[Irritant.GLUTEN])
        water = Basic("water")
        yeast = Basic("yeast")
        salt = Basic("salt")
        dough = Composite("dough", [water, flour, yeast, salt])
        dough_bad = dough.bad_ingredients(avoiding)
        self.assertEqual(dough_bad, [flour])
```

# Your task

Your task is to complete method 
`bad_ingredients` in classes `Basic`
and `Composite`.  

Do not change
`bad_ingredients` in the abstract base
class `Food`.  Do not change the
constructors of `Basic` and `Composite`.

## Hints

If you want to test whether any 
of the elements of a `list` are 
contained in a `set`, the simplest
approach is to convert the list to 
a set and use the `isdisjoint` method. 
Two sets are disjoint if their
intersection is empty: 

```commandline
>>> my_set = set(["a", "b"])
>>> my_set.isdisjoint(set(["b","c"]))
False
>>> my_set.isdisjoint(set(["c", "d"]))
True
```
## Turn in 

Turn in `food.py`

