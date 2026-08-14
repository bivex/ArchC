#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

COND_EQ = 0x0
COND_NE = 0x1
COND_AL = 0xE

def enc_dp(cond, cmd, s, rn, rd, op2):
    return ((cond & 0xF) << 28) | (0 << 26) | ((cmd & 0xF) << 21) | ((s & 1) << 20) | ((rn & 0xF) << 16) | ((rd & 0xF) << 12) | (op2 & 0xFFF)

def arm_add(rd, rn, op2, cond=COND_AL, s=0): return enc_dp(cond, 0x4, s, rn, rd, op2)
def arm_sub(rd, rn, op2, cond=COND_AL, s=0): return enc_dp(cond, 0x2, s, rn, rd, op2)
def arm_mov(rd, op2, cond=COND_AL, s=0): return enc_dp(cond, 0xD, s, 0, rd, op2)
def arm_cmp(rn, op2, cond=COND_AL): return enc_dp(cond, 0xA, 1, rn, 0, op2)
def arm_and(rd, rn, op2, cond=COND_AL): return enc_dp(cond, 0x0, 0, rn, rd, op2)
def arm_orr(rd, rn, op2, cond=COND_AL): return enc_dp(cond, 0xC, 0, rn, rd, op2)
def arm_eor(rd, rn, op2, cond=COND_AL): return enc_dp(cond, 0x1, 0, rn, rd, op2)

def arm_ldr(rd, rn, offset=0, cond=COND_AL):
    return ((cond & 0xF) << 28) | (1 << 26) | (1 << 20) | ((rn & 0xF) << 16) | ((rd & 0xF) << 12) | (offset & 0xFFF)

def arm_str(rd, rn, offset=0, cond=COND_AL):
    return ((cond & 0xF) << 28) | (1 << 26) | (0 << 20) | ((rn & 0xF) << 16) | ((rd & 0xF) << 12) | (offset & 0xFFF)

def arm_b(offset, cond=COND_AL):
    return ((cond & 0xF) << 28) | (5 << 25) | (0 << 24) | (offset & 0xFFFFFF)

def arm_hlt():
    return (0xE << 28) | (0xF << 24)

def write_hex(filename, instructions, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")

def write_elf(filename, instructions, base_addr=0x1000, is_elf64=False):
    code_bytes = b"".join(struct.pack("<I", x) for x in instructions)
    code_offset = 0x1000
    with open(filename, "wb") as f:
        if is_elf64:
            e_ident = b"\x7fELF\x02\x01\x01\x00" + b"\x00" * 8
            ehdr = struct.pack("<16sHHIQQQIHHHHHH", e_ident, 2, 40, 1,
                               base_addr, 64, 0, 0, 64, 56, 1, 64, 0, 0)
            phdr = struct.pack("<IIQQQQQQ", 1, 7, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 4)
            f.write(ehdr)
            f.write(phdr)
        else:
            e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
            ehdr = struct.pack("<16sHHIIIIIHHHHHH", e_ident, 2, 40, 1,
                               base_addr, 52, 0, 0, 52, 32, 1, 40, 0, 0)
            phdr = struct.pack("<IIIIIIII", 1, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 7, 4)
            f.write(ehdr)
            f.write(phdr)

        padding = code_offset - f.tell()
        if padding > 0:
            f.write(b"\x00" * padding)
        f.write(code_bytes)

def gen_arm_alu(outer_loops=5000, inner_loops=1000):
    instrs = []
    instrs.append(arm_mov(10, outer_loops))      # r10 = outer loop count
    instrs.append(arm_mov(1, 10))                # r1 = 10
    instrs.append(arm_mov(2, 20))                # r2 = 20
    instrs.append(arm_mov(3, 30))                # r3 = 30

    outer_start = len(instrs)
    instrs.append(arm_mov(0, inner_loops))       # r0 = inner loop count

    inner_start = len(instrs)
    instrs.append(arm_add(1, 1, 5))              # r1 = r1 + 5
    instrs.append(arm_sub(2, 2, 3))              # r2 = r2 - 3
    instrs.append(arm_eor(3, 3, 1))              # r3 = r3 ^ 1
    instrs.append(arm_orr(4, 1, 2))              # r4 = r1 | 2
    instrs.append(arm_and(5, 4, 3))              # r5 = r4 & 3
    instrs.append(arm_sub(0, 0, 1, s=1))         # r0 -= 1

    b_inner = len(instrs)
    instrs.append(arm_b(inner_start - b_inner, cond=COND_NE)) # bne inner_start

    instrs.append(arm_sub(10, 10, 1, s=1))       # r10 -= 1
    b_outer = len(instrs)
    instrs.append(arm_b(outer_start - b_outer, cond=COND_NE)) # bne outer_start

    instrs.append(arm_hlt())
    return instrs

def gen_arm_mem(outer_loops=4000, inner_loops=1000):
    instrs = []
    instrs.append(arm_mov(10, outer_loops))      # r10 = outer loop count
    instrs.append(arm_mov(1, 0x2000))            # r1 = base memory addr
    instrs.append(arm_mov(2, 42))                # r2 = 42

    outer_start = len(instrs)
    instrs.append(arm_mov(0, inner_loops))       # r0 = inner loop count

    inner_start = len(instrs)
    instrs.append(arm_str(2, 1, offset=0))       # mem[r1+0] = r2
    instrs.append(arm_ldr(3, 1, offset=0))       # r3 = mem[r1+0]
    instrs.append(arm_str(3, 1, offset=4))       # mem[r1+4] = r3
    instrs.append(arm_ldr(4, 1, offset=4))       # r4 = mem[r1+4]
    instrs.append(arm_sub(0, 0, 1, s=1))         # r0 -= 1

    b_inner = len(instrs)
    instrs.append(arm_b(inner_start - b_inner, cond=COND_NE))

    instrs.append(arm_sub(10, 10, 1, s=1))
    b_outer = len(instrs)
    instrs.append(arm_b(outer_start - b_outer, cond=COND_NE))

    instrs.append(arm_hlt())
    return instrs

def main():
    print("=" * 65)
    print(" ArchC ARM Architecture Model Verification & Performance Suite")
    print("=" * 65)

    alu_code = gen_arm_alu(4000, 4000)
    mem_code = gen_arm_mem(3000, 4000)

    write_hex("arm_alu.hex", alu_code, base_addr=0x1000)
    write_hex("arm_mem.hex", mem_code, base_addr=0x1000)
    write_elf("arm_alu.elf", alu_code, base_addr=0x1000, is_elf64=True)
    write_elf("arm_mem.elf", mem_code, base_addr=0x1000, is_elf64=True)

    sim = "./arm.x"
    test_files = ["arm_alu.hex", "arm_mem.hex", "arm_alu.elf", "arm_mem.elf"]

    for tf in test_files:
        p = subprocess.run([sim, f"--load={tf}"], capture_output=True, text=True)
        combined = p.stdout + p.stderr
        instr_line = "OK"
        speed_line = ""
        for l in combined.split("\n"):
            if "Number of instructions executed:" in l:
                instr_line = l.strip()
            elif "Simulation speed:" in l:
                speed_line = l.strip()
        print(f"  {tf:<14} | {instr_line:<45} | {speed_line}")

if __name__ == "__main__":
    main()
