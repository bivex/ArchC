#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def enc_xo(op, rt, ra, rb, oe, xo):
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | ((rb & 0x1F) << 11) | ((oe & 1) << 10) | (xo & 0x3FF)

def enc_d(op, rt, ra, d):
    return ((op & 0x3F) << 26) | ((rt & 0x1F) << 21) | ((ra & 0x1F) << 16) | (d & 0xFFFF)

def enc_b(op, bo, bi, bd):
    return ((op & 0x3F) << 26) | ((bo & 0x1F) << 21) | ((bi & 0x1F) << 16) | ((bd & 0x3FFF) << 2)

def ppc_add(rt, ra, rb): return enc_xo(31, rt, ra, rb, 0, 266)
def ppc_subf(rt, ra, rb): return enc_xo(31, rt, ra, rb, 0, 40)
def ppc_and(ra, rt, rb): return enc_xo(31, rt, ra, rb, 0, 28)
def ppc_or(ra, rt, rb): return enc_xo(31, rt, ra, rb, 0, 444)
def ppc_xor(ra, rt, rb): return enc_xo(31, rt, ra, rb, 0, 316)

def ppc_addi(rt, ra, d): return enc_d(14, rt, ra, d)
def ppc_addic_dot(rt, ra, d): return enc_d(13, rt, ra, d)
def ppc_addis(rt, ra, d): return enc_d(15, rt, ra, d)
def ppc_ori(ra, rt, d): return enc_d(24, rt, ra, d)
def ppc_andi_dot(ra, rt, d): return enc_d(28, rt, ra, d)
def ppc_lwz(rt, ra, d): return enc_d(32, rt, ra, d)
def ppc_stw(rt, ra, d): return enc_d(36, rt, ra, d)

def ppc_bne(bd): return enc_b(16, 4, 0, bd)
def ppc_halt(): return (63 << 26)

def write_hex(filename, instructions, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")

def write_elf(filename, instructions, base_addr=0x1000, is_elf64=True):
    code_bytes = b"".join(struct.pack(">I", x) for x in instructions)
    code_offset = 0x1000
    with open(filename, "wb") as f:
        if is_elf64:
            e_ident = b"\x7fELF\x02\x02\x01\x00" + b"\x00" * 8 # PPC64 Big Endian
            ehdr = struct.pack(">16sHHIQQQIHHHHHH", e_ident, 2, 21, 1,
                               base_addr, 64, 0, 0, 64, 56, 1, 64, 0, 0)
            phdr = struct.pack(">IIQQQQQQ", 1, 7, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 4)
            f.write(ehdr)
            f.write(phdr)
        else:
            e_ident = b"\x7fELF\x01\x02\x01\x00" + b"\x00" * 8 # PPC32 Big Endian
            ehdr = struct.pack(">16sHHIIIIIHHHHHH", e_ident, 2, 20, 1,
                               base_addr, 52, 0, 0, 52, 32, 1, 40, 0, 0)
            phdr = struct.pack(">IIIIIIII", 1, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 7, 4)
            f.write(ehdr)
            f.write(phdr)

        padding = code_offset - f.tell()
        if padding > 0:
            f.write(b"\x00" * padding)
        f.write(code_bytes)

def gen_ppc_alu(iterations=5000000):
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(ppc_addis(1, 0, high))        # r1 = high << 16
    instrs.append(ppc_ori(1, 1, low))           # r1 = r1 | low
    instrs.append(ppc_addi(2, 0, 10))           # r2 = 10
    instrs.append(ppc_addi(3, 0, 20))           # r3 = 20
    instrs.append(ppc_addi(4, 0, 30))           # r4 = 30

    loop_start = len(instrs)
    instrs.append(ppc_add(2, 2, 3))             # r2 = r2 + r3
    instrs.append(ppc_subf(3, 3, 4))            # r3 = r4 - r3
    instrs.append(ppc_xor(4, 4, 2))             # r4 = r4 ^ r2
    instrs.append(ppc_or(5, 2, 3))              # r5 = r2 | r3
    instrs.append(ppc_and(6, 5, 4))             # r6 = r5 & r4
    instrs.append(ppc_addic_dot(1, 1, -1))      # r1 -= 1, updates CR0

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(ppc_bne(offset))

    instrs.append(ppc_halt())
    return instrs

def gen_ppc_mem(iterations=4000000):
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(ppc_addis(1, 0, high))
    instrs.append(ppc_ori(1, 1, low))
    instrs.append(ppc_addi(2, 0, 0x2000))
    instrs.append(ppc_addi(3, 0, 42))

    loop_start = len(instrs)
    instrs.append(ppc_stw(3, 2, 0))             # mem[r2+0] = r3
    instrs.append(ppc_lwz(4, 2, 0))             # r4 = mem[r2+0]
    instrs.append(ppc_stw(4, 2, 4))             # mem[r2+4] = r4
    instrs.append(ppc_lwz(5, 2, 4))             # r5 = mem[r2+4]
    instrs.append(ppc_addic_dot(1, 1, -1))

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(ppc_bne(offset))

    instrs.append(ppc_halt())
    return instrs

def main():
    print("=" * 65)
    print(" ArchC PowerPC (PPC32) Verification & Performance Suite")
    print("=" * 65)

    alu_code = gen_ppc_alu(5000000)
    mem_code = gen_ppc_mem(4000000)

    write_hex("ppc_alu.hex", alu_code, base_addr=0x1000)
    write_hex("ppc_mem.hex", mem_code, base_addr=0x1000)
    write_elf("ppc_alu.elf", alu_code, base_addr=0x1000, is_elf64=True)
    write_elf("ppc_mem.elf", mem_code, base_addr=0x1000, is_elf64=True)

    sim = "./powerpc.x"
    test_files = ["ppc_alu.hex", "ppc_mem.hex", "ppc_alu.elf", "ppc_mem.elf"]

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
        print(f"  {tf:<15} | {instr_line:<43} | {speed_line}")

if __name__ == "__main__":
    main()
