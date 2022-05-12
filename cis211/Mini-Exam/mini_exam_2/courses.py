"""Mini-exam: course conflict example
(very simplified to fit in mini-exam format)

Name - Ryan Heise
"""
from typing import List

class Course:
    """Any course, scheduled or web-based (asynchronous)"""

    def conflicts_with(self, other: "Course") -> bool:
        """Does this course have a time commitment that conflicts
        with other course?
        """
        raise NotImplementedError("conflicts_with not implemented")

    def busy_at(self, hour: int) -> bool:
        """True if this course has a time commitment at
        time 'hour'
        """
        raise NotImplementedError("busy_at not implemented")


class LectureCourse(Course):
    """A lecture course has a scheduled lecture time, which must
    not conflict with the lecture time of any other course a student
    is taking.
    """
    def __init__(self, title: str, hour: int):
        self._hour = hour  # Simplified model of class times
        self.title = title

    def conflicts_with(self, other: "Course") -> bool:
        """
        c1.conflicts_with(c2) should return true
            if and only if both courses are held in the same time slot
        """
        if type(other) == WebCourse:
            return False
        if self._hour == other._hour:
            return True
        else:
            return False

    def busy_at(self, hour: int) -> bool:
        """
        returns True if the course has a scheduled lecture at time t

        crwr.busy_at(15) should return True
        crwr.busy_at(8) should return False
        """
        if self._hour == hour:
            return True
        else:
            return False


class WebCourse(Course):
    """A web-based course has no scheduled lecture; it never
    conflicts with another course.
    """

    def __init__(self, title: str):
        self.title = title

    def conflicts_with(self, other: "Course") -> bool:
        """
        Always going to return False becuase there is no scheduled time for web courses
        """
        return False

    def busy_at(self, hour: int) -> bool:
        """
        Always going to return False becuase there is no scheduled time for web courses
        """
        return False


class Schedule:
    """Wraps a list of courses"""

    def __init__(self, courses: List[Course]):
        self._courses = courses

    def has_conflicts(self) -> bool:
        """
         It should return False if no two courses in the schedule conflict.
        """
        for item in range(len(self._courses)): # 0, 1, 2, 3, 4
            for i in range(item + 1, len(self._courses)): # 1, 2, 3, 4, 2, 3, 4, 3, 4, 4
                print(item, i)
                if self._courses[item].conflicts_with(self._courses[i]):
                    return True
        return False