# Mini-exam 2 (week 4)

This mini-exam is designed to check your understanding
of subclasses and inheritance, as well as
(once again, and not for the last time) loop design.  

## Courses

We will have an abstract base class `Course`, and
two concrete subclasses, `LectureCourse` and 
`WebCourse`.   A `LectureCourse` has a scheduled
lecture time, which we have simplified to an 
integer.  Every lecture takes exactly one hour, 
and is held on Monday.  No weird 10:15 to 11:45 
time slots for us!  For example, we might have
a creative writing course at 3pm: 

```python
crwr = LectureCourse("Inauguration Poetry", 15)
```

The lecture time is a *private* attribute (instance variable) 
of the `LectureCourse` object.  The constructor for a 
`LectureCourse` should be 

```python
    def __init__(self, title: str, hour: int):
        self._hour = hour  # Simplified model of class times
        self.title = title
```

A web course has no scheduled lecture time.  For 
example: 

```python
dance = WebCourse("Kata as Dance")
```

The constructor for a `WebCourse` object is 

```python
    def __init__(self, title: str):
        self.title = title
```


## Course Conflict Checking

Abstract class `Course` has two abstract methods, 
`busy_at` and `conflicts_with`, 
that must be implemented by `LectureCourse` and 
`WebCourse`.   Method `busy_at(t)` returns `True` 
if the course has a scheduled lecture at time `t`. 
For the examples above, `crwr.busy_at(15)` should 
return `True`, `crwr.busy_at(8)` should return 
`False`, and `dance.busy_at(10)` should return
`False`. 

If `c1` and `c2` are `Course` objects,  
`c1.conflicts_with(c2)` should return true if and only if both
courses are held in the same time slot.   Note that
a web course can never conflict with any other 
course of any kind.  Thus `crwr.conflicts_with(dance)`
should return `False`, and likewise 
`dance.conflicts_with(crwr)` should return `False`. 


# Schedules

A `Schedule` object is a simple wrapper for a list of `Course` objects.
It's constructor looks like this: 

```python
    def __init__(self, courses: List[Course]):
        self._courses = courses
```

## Schedule conflict checking 

`Schedule` has a method `has_conflicts` which should return `True`
if any `Course` object in the schedule conflicts with any other 
`Course` object in the schedule.  It should return `False` if 
no two courses in the schedule conflict.  You must write this 
method. 

## Test files 

I have provided `test_courses.py` to test your `Course` class 
and `test_schedules.py` to test your `Schedule` class. You might
also find these files useful to help understand how your classes
are used. 

# Summary of what you must provide

Some starter code is provided `courses.py`.  The code you must fill 
in is

- method `busy_at` in classes `LectureCourse` and `WebCourse`. 
- method `conflicts_with` in classes `LectureCourse` and `WebCourse`.
- method `has_conflicts` in class `Schedule`

Do not change the headers of these methods. In class `Schedule`, do 
not access private instance variables of class `LectureCourse` or 
class `WebCourse`.  

When you have successfully completed this code, all the test cases 
in `test_courses.py` and `test_schedules.py` should pass. 

