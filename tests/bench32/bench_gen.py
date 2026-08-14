#!/usr/bin/env python3
import time
import subprocess
import os

def encode_r(op, rs, rt, rd, shamt, func):
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | ((rd & 0x1F) << 11) | ((shamt & 0x1F) << 6) | (func & 0x3F)

def encode_i(op, rs, rt, imm):
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | (imm & 0xFFFF)

def encode_j(op, addr):
    return ((op & 0x3F) << 26) | (addr & 0x3FFFFFF)

# Instruction encoders for bench32
def add(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x20)
def sub(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x22)
def and_op(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x24)
def or_op(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x25)
def xor_op(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x26)
def slt(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x2A)
def sll(rd, rt, shamt): return encode_r(0x00, 0, rt, rd, shamt, 0x00)
def srl(rd, rt, shamt): return encode_r(0x00, 0, rt, rd, shamt, 0x02)
def nop(): return encode_r(0x00, 0, 0, 0, 0, 0x01)

def addi(rt, rs, imm): return encode_i(0x08, rs, rt, imm)
def andi(rt, rs, imm): return encode_i(0x0C, rs, rt, imm)
def ori(rt, rs, imm): return encode_i(0x0D, rs, rt, imm)
def lw(rt, rs, imm): return encode_i(0x23, rs, rt, imm)
def sw(rt, rs, imm): return encode_i(0x2B, rs, rt, imm)
def beq(rs, rt, imm): return encode_i(0x04, rs, rt, imm)
def bne(rs, rt, imm): return encode_i(0x05, rs, rt, imm)

def j(addr): return encode_j(0x02, addr)
def halt(): return encode_j(0x3F, 0)

def write_hex_file(filename, instructions, base_addr=0):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")

def gen_alu_benchmark(iterations=1000000):
    """
    Loop running arithmetic instructions.
    R1: loop counter (iterations)
    """
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(addi(1, 0, high))       # r1 = high
    instrs.append(sll(1, 1, 16))          # r1 = r1 << 16
    instrs.append(ori(1, 1, low))         # r1 = r1 | low

    instrs.append(addi(2, 0, 1))          # r2 = 1
    instrs.append(addi(3, 0, 2))          # r3 = 2
    instrs.append(addi(4, 0, 3))          # r4 = 3
    instrs.append(addi(5, 0, 4))          # r5 = 4

    loop_start = len(instrs)              # index 7
    instrs.append(add(2, 2, 3))           # r2 = r2 + r3
    instrs.append(sub(3, 3, 4))           # r3 = r3 - r4
    instrs.append(and_op(4, 4, 5))        # r4 = r4 & r5
    instrs.append(or_op(5, 5, 2))         # r5 = r5 | r2
    instrs.append(xor_op(2, 2, 4))        # r2 = r2 ^ r4
    instrs.append(slt(6, 2, 3))           # r6 = r2 < r3
    instrs.append(addi(1, 1, -1))         # r1 = r1 - 1
    
    bne_idx = len(instrs)                 # index 14
    offset = loop_start - bne_idx         # offset in instructions = -7
    instrs.append(bne(1, 0, offset))

    instrs.append(halt())
    return instrs

def gen_mem_benchmark(iterations=500000):
    """
    Loop performing memory load and stores.
    """
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(addi(1, 0, high))
    instrs.append(sll(1, 1, 16))
    instrs.append(ori(1, 1, low))

    instrs.append(addi(2, 0, 0x1000))     # Base address 0x1000
    instrs.append(addi(3, 0, 42))         # Value 42

    loop_start = len(instrs)
    instrs.append(sw(3, 2, 0))            # mem[r2+0] = r3
    instrs.append(lw(4, 2, 0))            # r4 = mem[r2+0]
    instrs.append(sw(4, 2, 4))            # mem[r2+4] = r4
    instrs.append(lw(5, 2, 4))            # r5 = mem[r2+4]
    instrs.append(addi(1, 1, -1))         # r1 = r1 - 1

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(bne(1, 0, offset))

    instrs.append(halt())
    return instrs

def gen_branch_benchmark(iterations=1000000):
    """
    Loop with branching logic.
    """
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(addi(1, 0, high))
    instrs.append(sll(1, 1, 16))
    instrs.append(ori(1, 1, low))
    instrs.append(addi(2, 0, 0))

    loop_start = len(instrs)
    # Check if r1 is even/odd using andi
    instrs.append(andi(3, 1, 1))          # r3 = r1 & 1
    beq_idx = len(instrs)
    # if r3 == 0 (even), jump to even handler (+3)
    instrs.append(beq(3, 0, 3))           # jump over odd handler
    # odd handler:
    instrs.append(addi(2, 2, 1))          # r2 += 1
    j_idx = len(instrs)
    instrs.append(beq(0, 0, 2))           # jump to loop end
    # even handler:
    instrs.append(addi(2, 2, 2))          # r2 += 2
    # loop end:
    instrs.append(addi(1, 1, -1))         # r1 -= 1
    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(bne(1, 0, offset))

    instrs.append(halt())
    return instrs

if __name__ == "__main__":
    alu_prog = gen_alu_benchmark(1000000)
    write_hex_file("alu_bench.hex", alu_prog, base_addr=0x1000)
    print(f"[+] Generated alu_bench.hex (~8M dynamic instructions) at 0x1000")

    mem_prog = gen_mem_benchmark(500000)
    write_hex_file("mem_bench.hex", mem_prog, base_addr=0x1000)
    print(f"[+] Generated mem_bench.hex (~3M dynamic instructions) at 0x1000")

    br_prog = gen_branch_benchmark(1000000)
    write_hex_file("branch_bench.hex", br_prog, base_addr=0x1000)
    print(f"[+] Generated branch_bench.hex (~6M dynamic instructions) at 0x1000")
