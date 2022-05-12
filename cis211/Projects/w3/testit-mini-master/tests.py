"""Unit tests for testme.py"""

import unittest
from buggy import *

class TestMaxRun(unittest.TestCase):

    def test_max_run(self):
        self.assertEqual(max_run([1, 1, 2, 2, 2, 3, 3, 3, 3]), [3, 3, 3, 3])


class TestEmptyCase(unittest.TestCase):
    def test_empty_list(self):
        self.assertEqual(max_run([]), [])

if __name__ == "__main__":
    unittest.main()