"""Test cases for schedules (collections of courses)"""

from courses import *
import unittest


class Schedule_Test(unittest.TestCase):

    def test_simplest_conflict(self):
        """Just two courses, but at the same time"""
        sched = Schedule([
            LectureCourse("Cinema and Brain Health", 15),
            LectureCourse("Cinema in Medieval Europe", 15)
        ])
        self.assertTrue(sched.has_conflicts())

    def test_empty_schedule(self):
        sched = Schedule([])
        self.assertFalse(sched.has_conflicts())

    def test_conflicting_schedule(self):
        """There is a conflict between two courses at 11"""
        sched = Schedule([
            WebCourse("History of Chocolate"),
            LectureCourse("Chocolate in Music", 11),
            LectureCourse("Chocolate and Health", 15),
            LectureCourse("Labor Issues in Cocoa Production", 11),
            WebCourse("Geography of Chocolate")
        ])
        self.assertTrue(sched.has_conflicts())

    def test_clear_schedule(self):
        """No conflicts in this schedule"""
        sched = Schedule([
            WebCourse("History of the Trombone"),
            LectureCourse("Trombones in New Orleans Culture", 11),
            LectureCourse("Sad Trombone: Deconstruction of a Meme", 15),
            LectureCourse("Trombone Production in Latin America", 10),
            WebCourse("Health Risks of Trombone Practice")
        ])

if __name__ == "__main__":
    """unittest.main runs everything with "test" in its name"""
    unittest.main()