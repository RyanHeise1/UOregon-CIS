"""Test the course scheduling classes"""
from courses import *
import unittest

class Courses_Test(unittest.TestCase):
    """Behavior of individual courses"""
    def test_simplest_conflict(self):
        """Two lecture courses scheduled the same hour"""
        algebra = LectureCourse("algebra", 8)
        geometry = LectureCourse("geometry", 8)
        self.assertTrue(algebra.conflicts_with(geometry))

    def test_diff_lecture_times(self):
        """Two lecture courses at different hours.
        These are real class titles at UO, and both are
        good courses.
        """
        music = LectureCourse("Music and the Brain", 12)
        aliens = LectureCourse("Aliens and Anthropology", 15)
        self.assertFalse(music.conflicts_with(aliens))

    def test_web_no_conflict(self):
        """Web courses have no scheduled lecture, so they
        don't conflict with scheduled lecture courses.
        """
        writing = LectureCourse("Writing 101", 15)
        anthro = WebCourse("Intro Cultural Anthropology")
        dance = WebCourse("Juke and Tango: An Unlikely Combination")
        self.assertFalse(writing.conflicts_with(anthro))
        self.assertFalse(anthro.conflicts_with(writing))
        self.assertFalse(dance.conflicts_with(anthro))


if __name__ == "__main__":
    """unittest.main runs everything with "test" in its name"""
    unittest.main()