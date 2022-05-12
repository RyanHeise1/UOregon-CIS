"""
Week 3 Project - Contagion
Author - Ryan Heise

Test simulation model of a disease spreading

This is NOT the main file. Just all of the important code that makes it run
To run this simulation, you must run the contagion.py file
"""


# Import statements
import mvc
import config
import enum
from typing import List, Tuple
import random
import logging
logging.basicConfig()
log= logging.getLogger(__name__)
log.setLevel(logging.DEBUG)



class Health(enum.Enum):
    """Each individual is one discrete state of health"""
    vulnerable = enum.auto()
    asymptomatic = enum.auto()
    symptomatic = enum.auto()
    recovered = enum.auto()
    dead = enum.auto()

    def __str__(self) -> str:
        return self.name



class Individual(mvc.Listenable):
    """An individual in the population,
    e.g., a person who might get and spread a disease.
    The 'state' instance variable is public read-only, e.g.,
    listeners can check it.
    """

    def __init__(self, kind: str,
                 region: "Population", row: int, col: int):
        # Listener needs its own initialization
        super().__init__()
        self.kind = kind
        self.region = region
        self.row = row
        self.col = col
        # Initially we are 'vulnerable', not yet infected
        self._time_in_state = 0  # How long in this state?
        self.state = Health.vulnerable
        self.next_state = Health.vulnerable
        # Configuration parameters based on kind
        self.T_Incubate = config.get_int(kind, "T_Incubate")
        self.P_Transmit = config.get_float(kind, "P_Transmit")
        self.T_Recover = config.get_int(kind, "T_Recover")
        self.P_Death = config.get_float(kind, "P_Death")
        self.P_Greet = config.get_float(kind, "P_Greet")
        self.N_Neighbors = config.get_int(kind, "N_Neighbors")
        self.P_Visit = config.get_float(kind, "P_Visit")
        self.Visit_Dist = config.get_int(kind, "Visit_Dist")
        self.neighbors = region.neighbors(num=self.N_Neighbors,
                                          row=row, col=col,
                                          dist=self.Visit_Dist)

    def step(self):
        """Next state"""
        # Basic state transitions are in common
        if self.state == Health.asymptomatic:
            if self._time_in_state > self.T_Incubate:
                self.next_state = Health.symptomatic
                log.debug("Becoming symptomatic")
        if self.state == Health.symptomatic:
            # We could die on any time step before we recover
            if self._time_in_state > self.T_Recover:
                log.debug(f"Recovery at {self.row},{self.col}")
                self.next_state = Health.recovered
            elif random.random() < self.P_Death:
                log.debug(f"Death at {self.row},{self.col}")
                self.next_state = Health.dead

        # Social behavior differs among concrete classes
        self.social_behavior()

    def tick(self):
        """Time passes"""
        self._time_in_state += 1
        if self.state != self.next_state:
            self.state = self.next_state
            self.notify_all("newstate")
            # Reset clock
            self._time_in_state = 0

    def infect(self):
        """Called by another individual spreading germs.
        May also be called on "patient 0" to start simulation.
        """
        if self.state == Health.vulnerable:
            self.next_state = Health.asymptomatic

    def _is_contagious(self) -> bool:
        """SARS COVID 19 apparently spreads before
        the individual is symptomatic.
        """
        return (self.state == Health.symptomatic
                or self.state == Health.asymptomatic)

    def maybe_transmit(self, other: "Individual"):
        if not self._is_contagious():
            return
        if not other.state == Health.vulnerable:
            return
        # Transmission is possible.  Roll the dice
        if random.random() < self.P_Transmit:
            other.infect()

    def meet(self, other: "Individual"):
        """
        Two individuals meet.  Either may infect
        the other.
        """
        self.maybe_transmit(other)  # I might infect you
        other.maybe_transmit(self)  # You might infect me

    def hello(self, visitor: "Individual") -> bool:
        """True means 'welcome' and False means 'go away'"""
        raise NotImplementedError("Each class must implement 'hello'")

    def social_behavior(self):
        pass

class Population(mvc.Listenable):
    """Simple grid organization of individuals"""

    def __init__(self, rows: int, cols: int):
        super().__init__()
        self.cells = []
        self.nrows = rows
        self.ncols = cols
        # Populate according to configuration
        for row_i in range(config.get_int("Grid", "Rows")):
            row = []
            for col_i in range(config.get_int("Grid", "Cols")):
                row.append(self._random_individual(row_i, col_i))
            self.cells.append(row)
        return

    def visit(self, address: Tuple[int, int]):
        """Who lives there?"""
        row_num, col_num = address
        return self.cells[row_num][col_num]

    def _random_individual(self, row: int, col: int) -> "Individual":
        classes = [(AtRisk, config.get_float("Grid", "Proportion_AtRisk")),
                   (Typical, config.get_float("Grid", "Proportion_Typical"))]
        while True:
            for the_class, proportion in classes:
                dice = random.random()
                if dice < proportion:
                    return the_class(self, row, col)

    def step(self):
        """Determine next states"""
        log.debug("Population: Step")
        # Time passes
        for row in self.cells:
            for cell in row:
                cell.step()
        for row in self.cells:
            for cell in row:
                cell.tick()
        self.notify_all("timestep")

    def seed(self):
        """Patient zero"""
        row = random.randint(0, self.nrows-1)
        col = random.randint(0, self.ncols-1)
        self.cells[row][col].infect()
        self.cells[row][col].tick()

    def count_in_state(self, state: Health) -> int:
        """How many individuals are currently in state"""
        ct = 0
        for row in range(self.nrows):
            for col in range(self.ncols):
                if state == self.cells[row][col].state:
                    ct += 1
        return ct

    def neighbors(self, num: int, row: int, col: int, dist: int) -> List[Tuple[int, int]]:
        """Give me addresses of up to num neighbors
        up to dist away from here(Manhattan distance)
        """
        result = []
        count = 0
        log.debug(f"Cell {row},{col} finding {num} neighbors at distance {dist} " +
                  f"in {self.nrows},{self.ncols}")
        attempts = 0
        while count < num:
            attempts += 1
            assert attempts < 1000, (
                f"Can't find {num} neighbors at distance {dist}")
            row_step = random.randint(0 - dist, dist)
            col_step = random.randint(0 - dist, dist)
            row_addr = row + row_step
            col_addr = col + col_step
            # log.debug(f"Trying neighbor at position {row_addr},{col_addr}")
            if row_addr < 0 or row_addr >= self.nrows:
                # log.debug("Bad row")
                continue
            if col_addr < 0 or col_addr >= self.ncols:
                # log.debug("Bad column")
                continue
            if row_addr == row and col_addr == 0:
                # log.debug("Can't visit self")
                continue
            neighbor_addr = (row_addr, col_addr)
            if neighbor_addr in result:
                continue
            log.debug(f"{row},{col} adding neighbor at {row_addr},{col_addr}")
            result.append(neighbor_addr)
            count += 1
        return result



class Typical(Individual):
    """Typical individual. May visit different neighbors
    each day.
    """
    def __init__(self, region: Population, row: int, col: int):
        # Much of the constructor has been "factored out" into
        # the abstract base class
        super().__init__("Typical", region, row, col)
        self.prior_visit = None

    def social_behavior(self):
        """
        When a typical individual makes a visit (at most one each day), they simply choose a neighbor at random:
        """
        if random.random() >= self.P_Visit:
            return
        addy = random.choice(self.neighbors)
        neighbor = self.region.visit(addy)
        if neighbor.hello(self):
            neighbor.meet(self)

    def hello(self, visitor: "Individual") -> bool:
        """Always welcomes people :D (Even if they dont know them)"""
        return True



class AtRisk(Individual):
    """Immunocompromised or elderly.
    Vulnerable and cautious.
    """
    def __init__(self, region: "Population", row: int, col: int):
        # Much of the constructor has been "factored out" into
        # the abstract base class
        super().__init__("AtRisk", region, row, col)
        self.prior_visit = None

    def hello(self, visitor: "Individual") -> bool:
        """True means 'welcome' and False means 'go away'"""
        return (visitor.row, visitor.col) in self.neighbors

    def social_behavior(self):
        if random.random() >= self.P_Visit:
            return
        if self.prior_visit is None:
            new = random.choice(self.neighbors)
            neighbor = self.region.visit(new)
            self.prior_visit = neighbor
        else:
            neighbor = self.prior_visit
            self.prior_visit = None
        if neighbor.hello(self):
            neighbor.meet(self)

class Wanderer(Individual):
    """
    The Wanderer's ignore warnings to keep close to home and traveled widely
    """
    def __init__(self, region: "Population", row: int, col: int):
        # Much of the constructor has been "factored out" into
        # the abstract base class
        super().__init__("Wanderer", region, row, col)
        self.prior_visit = None

    def hello(self, visitor: "Individual") -> bool:
        """Always welcomes people into their house (x_pos and y_pos)  (Even if they dont know them)"""
        return True

    def social_behavior(self):
        """Social behavior for an individual that moves around a lot (ie: trumpers)
        they are basically people that dont care about the virus"""
        if random.random() >= self.P_Visit:
            return
        if self.prior_visit is None:
            new = random.choice(self.neighbors)
            neighbor = self.region.visit(new)
            self.prior_visit = neighbor
        else:
            neighbor = self.prior_visit
            self.prior_visit = None
        if neighbor.hello(self):
            neighbor.meet(self)