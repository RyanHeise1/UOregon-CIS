"""Lab05 CIS 211 2/3/21"""

import copy
from typing import List, Set, Dict, Optional

class Student:

    def __init__(self, name: str,
                 interests: List[str]):
        self.name = name
        self.interests = interests
        self.freetimes = set([8, 9, 10, 11, 12, 13, 14, 15])
        self.meetings: List[int] = []

    def schedule_meeting(self, time: int):
        if time in self.freetimes:
            self.meetings.append(time)
            self.freetimes.remove(time)


class Club:

    def __init__(self, name: str):
        self.name = name
        self.members: List[Student] = []
        self.meeting_time: Optional[int] = None

    def __str__(self) -> str:
        member_names = [member.name for member in self.members]
        return f"{self.name} ({', '.join(member_names)})"

    def join(self, student: Student):
        self.members.append(student)

    def find_common_time(self) -> int:
        if len(self.members) == 0:
            return 0
        common_times = self.members[0].freetimes.copy()
        for student in self.members:
            for time in common_times:
                if len(common_times) == 0:
                    return 0
                if time not in student.freetimes:
                    common_times.remove(time)
        return common_times.pop()

    def schedule(self, time: int):
        self.meeting_time = time
        for student in self.members:
            student.schedule_meeting(time)



class ASUO:

    def __init__(self):
        self.students: List[Student] = []
        self.clubs: List[Club] = []

    def enroll(self, s: Student):
        self.students.append(s)

    def form_clubs(self):
        clubs_to_form: Dict[str, Club] = {}
        for student in self.students:
            for intrest in student.interests:
                if intrest not in clubs_to_form.keys():
                    clubs_to_form[intrest] = Club(intrest)
                    clubs_to_form[intrest].join(student)
                else:
                    clubs_to_form[intrest].join(student)

        self.clubs = clubs_to_form.values()

    def schedule_clubs(self):
        for club in self.clubs:
            common_time = club.find_common_time()
            if common_time > 0:
                club.schedule(common_time)

    def print_club_schedule(self):
        for club in self.clubs:
            if club.meeting_time is not None:
                print(f"{club} meets at {club.meeting_time}")

    def __repr__(self) -> str:
        return f"{self.name}"



asuo = ASUO()
asuo.enroll(Student("Marty", ["badminton", "robotics"]))
asuo.enroll(Student("Kim", ["backgammon"]))
asuo.enroll(Student("Tara", ["robotics", "horticulture", "chess"]))
asuo.enroll(Student("George", ["chess", "badminton"]))

asuo.form_clubs()
asuo.schedule_clubs()
asuo.print_club_schedule()