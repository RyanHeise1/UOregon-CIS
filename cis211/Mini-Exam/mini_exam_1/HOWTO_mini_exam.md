# Mini-exam 1 HOWTO

This mini-exam requires you
to build one class, called 
Room, and to write one 
function that uses that class.
Both parts should be written in one file, 
`mini_exam.py`. 

# Q1: class Room

Construct a class `Room`.

A `Room` object has two instance variables,
a building (represented by a string) and a
room number (represented by an integer).  
For example, we can build Room for Deschutes 243
as `Room("Deschutes", 243)`.  

The printed representation of a `Room` object is
a string consisting of the building name, one space,
and the building number.  For example, if we write
```python
des243 = Room("Deschutes", 243)
```
then `str(des243)` should be `"Deschutes 243"`. 

`Room` has a method 
```python
    def same_building(self, other: "Room") -> bool:
        """These two rooms are in the same building"""
```
For example, if we have 
```python
des243 = Room("Deschutes", 243)
des150 = Room("Deschutes", 160)
straub150 = Room("Straub", 150)
```
then `des150.same_building(des243)` should return `True`,
but `des150.same_building(straub150)` should return `False`.

File `test_q1.py` contains test cases for class `Room`. 

## Q2: Passing between classes 

Once upon a time, in a university far away, students
went to physical classrooms.  Sometimes they could 
get from one class to another within the same building, 
and did not have to put on coats even when it was raining. 
Often they had to leave one building and walk to another. 

A student's daily schedule would depend on how many
times they had to go out of one building to reach another. 
For example, if a student had a class in Straub Hall, 
then a class in Willamette Hall, then another class in 
Willamette Hall, and then another in Chapman Hall, 
and then another in Chapman Hall, they would need to 
go outside two times:  Once to get from Straub to Willamette,
again to get from Willamette to Chapman.  (We count only
trips from one class to another, not the initial trip to 
campus in the morning nor the trip home at the end of classes.)

Write function 
```python
outdoor_passing(classes: List[Room]) -> int
```
to count the number of times a student must go outdoors
to get through a list of classes.  
We use the `same_building` method of class `Room`
to determine whether or not we passing from one 
classroom to another will require going outdoors. 
For example, if we 
have 
```python
tuesday = [Room("Straub", 150), 
           Room("Willamette", 150), 
           Room("Willamette", 238),
           Room("Chapman", 218),
           Room("Chapman", 13)
           ]
```
then `outside_passing(tuesday)` should return 2. 

Note that the list of classes for a day might be 
empty, and `outside_passing([])` should return 0. 

File `test_q2.py` contains test cases for your 
`outdoor_passing` function. 





