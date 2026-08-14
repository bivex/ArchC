#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def enc_rr(opcode, mod, reg, rm):
    return ((opcode & 0xFF) << 24) | ((mod & 0x3) << 22) | ((reg & 0x7) << 19) | ((rm & 0x7) << 16)

def enc_ri(opcode, reg, imm):
    return ((opcode & 0xFF) << 24) | ((reg & 0x7) << 21) | (imm & 0xFFFF)

def enc_rm(opcode, mod, reg, rm, disp):
    return ((opcode & 0xFF) << 24) | ((mod & 0x3) << 22) | ((reg & 0x7) << 19) | ((rm & 0x7) << 16) | (disp & 0xFFFF)

def enc_j(opcode, offset):
    return ((opcode & 0xFF) << 24) | (offset & 0xFFFFFF)

def x86_add_rr(rm, reg): return enc_rr(0x01, 3, reg, rm)
def x86_sub_rr(rm, reg): return enc_rr(0x29, 3, reg, rm)
def x86_and_rr(rm, reg): return enc_rr(0x21, 3, reg, rm)
def x86_or_rr(rm, reg):  return enc_rr(0x09, 3, reg, rm)
def x86_xor_rr(rm, reg): return enc_rr(0x31, 3, reg, rm)

def x86_mov_ri(reg, imm): return enc_ri(0xB8, reg, imm)
def x86_sub_ri(reg, imm): return enc_ri(0x83, reg, imm)
def x86_cmp_ri(reg, imm): return enc_ri(0x3D, reg, imm)

def x86_mov_rm(reg, rm, disp=0): return enc_rm(0x8B, 2, reg, rm, disp)
def x86_mov_mr(rm, reg, disp=0): return enc_rm(0x88, 2, reg, rm, disp)

def x86_jne(offset): return enc_j(0x75, offset)
def x86_halt(): return (0xF4 << 24)

def write_hex(filename, instructions, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")

def write_elf(filename, instructions, base_addr=0x1000, is_elf64=True):
    code_bytes = b"".join(struct.pack("<I", x) for x in instructions)
    code_offset = 0x1000
    with open(filename, "wb") as f:
        if is_elf64:
            e_ident = b"\x7fELF\x02\x01\x01\x00" + b"\x00" * 8 # x86_64 LE
            ehdr = struct.pack("<16sHHIQQQIHHHHHH", e_ident, 2, 62, 1,
                               base_addr, 64, 0, 0, 64, 56, 1, 64, 0, 0)
            phdr = struct.pack("<IIQQQQQQ", 1, 7, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 4)
            f.write(ehdr)
            f.write(phdr)
        else:
            e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8 # i386 LE
            ehdr = struct.pack("<16sHHIIIIIHHHHHH", e_ident, 2, 3, 1,
                               base_addr, 52, 0, 0, 52, 32, 1, 40, 0, 0)
            phdr = struct.pack("<IIIIIIII", 1, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 7, 4)
            f.write(ehdr)
            f.write(phdr)

        padding = code_offset - f.tell()
        if padding > 0:
            f.write(b"\x00" * padding)
        f.write(code_bytes)

def gen_x86_alu(iterations=5000000):
    # Use nested loops: 5000 * 1000 = 5,000,000
    instrs = []
    instrs.append(x86_mov_ri(7, 5000))          # edi = 5000 (outer loop)
    instrs.append(x86_mov_ri(1, 10))            # ecx = 10
    instrs.append(x86_mov_ri(2, 20))            # edx = 20
    instrs.append(x86_mov_ri(3, 30))            # ebx = 30

    outer_start = len(instrs)
    instrs.append(x86_mov_ri(0, 1000))          # eax = 1000 (inner loop)

    inner_start = len(instrs)
    instrs.append(x86_add_rr(1, 2))             # ecx += edx
    instrs.append(x86_sub_rr(2, 3))             # edx -= ebx
    instrs.append(x86_xor_rr(3, 1))             # ebx ^= ecx
    instrs.append(x86_or_rr(4, 1))              # esp |= ecx
    instrs.append(x86_and_rr(5, 2))             # ebp &= edx
    instrs.append(x86_sub_ri(0, 1))             # eax -= 1

    b_inner = len(instrs)
    instrs.append(x86_jne(inner_start - b_inner))

    instrs.append(x86_sub_ri(7, 1))             # edi -= 1
    b_outer = len(instrs)
    instrs.append(x86_jne(outer_start - b_outer))

    instrs.append(x86_halt())
    return instrs

def gen_x86_mem(iterations=4000000):
    instrs = []
    instrs.append(x86_mov_ri(7, 4000))          # edi = 4000 (outer)
    instrs.append(x86_mov_ri(1, 0x2000))        # ecx = base addr
    instrs.append(x86_mov_ri(2, 42))            # edx = 42

    outer_start = len(instrs)
    instrs.append(x86_mov_ri(0, 1000))          # eax = 1000 (inner)

    inner_start = len(instrs)
    instrs.append(x86_mov_mr(1, 2, disp=0))     # mem[ecx+0] = edx
    instrs.append(x86_mov_rm(3, 1, disp=0))     # ebx = mem[ecx+0]
    instrs.append(x86_mov_mr(1, 3, disp=4))     # mem[ecx+4] = ebx
    instrs.append(x86_mov_rm(4, 1, disp=4))     # esp = mem[ecx+4]
    instrs.append(x86_sub_ri(0, 1))

    b_inner = len(instrs)
    instrs.append(x86_jne(inner_start - b_inner))

    instrs.append(x86_sub_ri(7, 1))
    b_outer = len(instrs)
    instrs.append(x86_jne(outer_start - b_outer))

    instrs.append(x86_halt())
    return instrs

def main():
    print("=" * 65)
    print(" ArchC Intel x86 (i386) Verification & Performance Suite")
    print("=" * 65)

    alu_code = gen_x86_alu()
    mem_code = gen_x86_mem()

    write_hex("x86_alu.hex", alu_code, base_addr=0x1000)
    write_hex("x86_mem.hex", mem_code, base_addr=0x1000)
    write_elf("x86_alu.elf", alu_code, base_addr=0x1000, is_elf64=True)
    write_elf("x86_mem.elf", mem_code, base_addr=0x1000, is_elf64=True)

    sim = "./i386.x"
    test_files = ["x86_alu.hex", "x86_mem.hex", "x86_alu.elf", "x86_mem.elf"]

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
