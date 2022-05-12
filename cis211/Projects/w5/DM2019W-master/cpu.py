"""
Duck Machine model DM2020W CPU
Author: Ryan Heise
"""

from instr_format import Instruction, OpCode, CondFlag, decode
from typing import Tuple
from memory import Memory
from register import Register, ZeroRegister
from mvc import MVCEvent, MVCListenable
import logging
logging.basicConfig()
log = logging.getLogger(__name__)
log.setLevel(logging.INFO)


# -------------------------------------------------------------------
#                           ALU
# -------------------------------------------------------------------
class ALU(object):
    """The arithmetic logic unit (also called a "functional unit"
    in a modern CPU) executes a selected function but does not
    otherwise manage CPU state. A modern CPU core may have several
    ALUs to boost performance by performing multiple operatons
    in parallel, but the Duck Machine has just one ALU in one core.
    """
    # The ALU chooses one operation to apply based on a provided
    # operation code.  These are just simple functions of two arguments;
    # in hardware we would use a multiplexer circuit to connect the
    # inputs and output to the selected circuitry for each operation.
    ALU_OPS = {
        OpCode.ADD: lambda x, y: x + y,
        OpCode.DIV: lambda x, y: (x // y) if (y is not 0) else (0),
        OpCode.SUB: lambda x, y: x - y,
        OpCode.MUL: lambda x, y: x * y,
        # For memory access operations load, store, the ALU
        # performs the address calculation
        OpCode.LOAD: lambda x, y: x + y,
        OpCode.STORE: lambda x, y: x + y,
        # Some operations perform no operation
        OpCode.HALT: lambda x, y: 0
    }

    def exec(self, op: OpCode, in1: int, in2: int) -> Tuple[int, CondFlag]:
        if op == OpCode.DIV:
            if in2 is 0:
                cond_code = CondFlag.V

        total = self.ALU_OPS[op](in1, in2)

        if total < 0:
            cond_code = CondFlag.M
        elif total == 0:
            cond_code = CondFlag.Z
        elif total > 0:
            cond_code = CondFlag.P
        return total, cond_code


# -------------------------------------------------------------------
#                           CPU
# -------------------------------------------------------------------
class CPUStep(MVCEvent):
    """CPU is beginning step with PC at a given address"""
    def __init__(self, subject: "CPU", pc_addr: int,
                 instr_word: int, instr: Instruction)-> None:
        self.subject = subject
        self.pc_addr = pc_addr
        self.instr_word = instr_word
        self.instr = instr


class CPU(MVCListenable):
    """Duck Machine central processing unit (CPU)
    has 16 registers (including r0 that always holds zero
    and r15 that holds the program counter), a few
    flag registers (condition codes, halted state),
    and some logic for sequencing execution.  The CPU
    does not contain the main memory but has a bus connecting
    it to a separate memory.
    """

    def __init__(self, memory: Memory):
        super().__init__()
        self.memory = memory  # Not part of CPU; what we really have is a connection
        self.registers = [ZeroRegister(), Register(), Register(), Register(),
                          Register(), Register(), Register(), Register(),
                          Register(), Register(), Register(), Register(),
                          Register(), Register(), Register(), Register()]

        self.condition = CondFlag.ALWAYS
        self.halted = False

        # Preform ALU Actions
        self.alu = ALU()

        self.pc = self.registers[15]

    def step(self):
        # Fetch Step
        instr_addr = self.pc.get()
        read_word = self.memory.get(instr_addr)

        # Decode Step
        instr = decode(read_word)
        # Display the CPU state when we have decoded the instruction,
        # before we have executed it
        self.notify_all(CPUStep(self, instr_addr, read_word, instr))

        # Execute Step
        # Check the instruction predicate
        if instr.cond & self.condition:
            opcode = instr.op
            target = self.registers[instr.reg_target]
            # The left operand will be the contents of the register specified by instr.src1
            lft = self.registers[instr.reg_src1].get()
            # right operand will be the sum of the instr.offset field and the contents of the register specified by
            # instr.src2
            rgt = self.registers[instr.reg_src2].get() + instr.offset
            # increment the program counter (register 15)
            self.pc.put(self.pc.get() + 1)
            # calculate a result value and new condition code by calling the ALU exec method
            result, cc = self.alu.exec(opcode, lft, rgt)
            self.condition = cc
            # If operating was STORE we use the result of the calc as a memory address and save it to that location
            if opcode == OpCode.LOAD:
                memory_val = self.memory.get(result)
                target.put(memory_val)
            # If operation was LOAD we use the result of the calc as a memory address and fetch the value of that location
            elif opcode == OpCode.STORE:
                self.memory.put(result, target.get())
            # If operation was HALT we set the halt flag to True
            elif opcode == OpCode.HALT:
                self.halted = True
            else:
                target.put(result)
        else:
            # The program counter counts 1
            self.pc.put(self.pc.get() + 1)

    def run(self, from_addr=0, single_step=False) -> None:
        self.halted = False
        self.pc.put(from_addr)
        step_count = 0
        while not self.halted:
            if single_step:
                input("Step {}; press enter".format(step_count))
            self.step()
            step_count += 1