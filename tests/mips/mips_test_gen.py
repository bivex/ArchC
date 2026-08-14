#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def enc_r(op, rs, rt, rd, shamt, func):
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | ((rd & 0x1F) << 11) | ((shamt & 0x1F) << 6) | (func & 0x3F)

def enc_i(op, rs, rt, imm):
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | (imm & 0xFFFF)

def enc_j(op, addr):
    return ((op & 0x3F) << 26) | (addr & 0x3FFFFFF)

def mips_add(rd, rs, rt): return enc_r(0x00, rs, rt, rd, 0, 0x20)
def mips_sub(rd, rs, rt): return enc_r(0x00, rs, rt, rd, 0, 0x22)
def mips_and(rd, rs, rt): return enc_r(0x00, rs, rt, rd, 0, 0x24)
def mips_or(rd, rs, rt):  return enc_r(0x00, rs, rt, rd, 0, 0x25)
def mips_xor(rd, rs, rt): return enc_r(0x00, rs, rt, rd, 0, 0x26)
def mips_slt(rd, rs, rt): return enc_r(0x00, rs, rt, rd, 0, 0x2A)
def mips_sll(rd, rt, shamt): return enc_r(0x00, 0, rt, rd, shamt, 0x00)

def mips_addi(rt, rs, imm): return enc_i(0x08, rs, rt, imm)
def mips_ori(rt, rs, imm):  return enc_i(0x0D, rs, rt, imm)
def mips_lw(rt, rs, imm):   return enc_i(0x23, rs, rt, imm)
def mips_sw(rt, rs, imm):   return enc_i(0x2B, rs, rt, imm)
def mips_beq(rs, rt, imm):  return enc_i(0x04, rs, rt, imm)
def mips_bne(rs, rt, imm):  return enc_i(0x05, rs, rt, imm)

def mips_j(addr): return enc_j(0x02, addr)
def mips_halt(): return enc_j(0x3F, 0)

def write_hex(filename, instructions, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")

def write_elf(filename, instructions, base_addr=0x1000, is_elf64=True):
    # MIPS is Big Endian in this model
    code_bytes = b"".join(struct.pack(">I", x) for x in instructions)
    code_offset = 0x1000
    with open(filename, "wb") as f:
        if is_elf64:
            e_ident = b"\x7fELF\x02\x02\x01\x00" + b"\x00" * 8 # 64-bit Big Endian
            ehdr = struct.pack(">16sHHIQQQIHHHHHH", e_ident, 2, 8, 1,
                               base_addr, 64, 0, 0, 64, 56, 1, 64, 0, 0)
            phdr = struct.pack(">IIQQQQQQ", 1, 7, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 4)
            f.write(ehdr)
            f.write(phdr)
        else:
            e_ident = b"\x7fELF\x01\x02\x01\x00" + b"\x00" * 8 # 32-bit Big Endian
            ehdr = struct.pack(">16sHHIIIIIHHHHHH", e_ident, 2, 8, 1,
                               base_addr, 52, 0, 0, 52, 32, 1, 40, 0, 0)
            phdr = struct.pack(">IIIIIIII", 1, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 7, 4)
            f.write(ehdr)
            f.write(phdr)

        padding = code_offset - f.tell()
        if padding > 0:
            f.write(b"\x00" * padding)
        f.write(code_bytes)

def gen_mips_alu(iterations=5000000):
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(mips_addi(1, 0, high))
    instrs.append(mips_sll(1, 1, 16))
    instrs.append(mips_ori(1, 1, low))
    instrs.append(mips_addi(2, 0, 10))
    instrs.append(mips_addi(3, 0, 20))
    instrs.append(mips_addi(4, 0, 30))

    loop_start = len(instrs)
    instrs.append(mips_add(2, 2, 3))
    instrs.append(mips_sub(3, 3, 4))
    instrs.append(mips_xor(4, 4, 2))
    instrs.append(mips_or(5, 2, 3))
    instrs.append(mips_and(6, 5, 4))
    instrs.append(mips_slt(7, 2, 3))
    instrs.append(mips_addi(1, 1, -1))

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(mips_bne(1, 0, offset))

    instrs.append(mips_halt())
    return instrs

def gen_mips_mem(iterations=4000000):
    instrs = []
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    instrs.append(mips_addi(1, 0, high))
    instrs.append(mips_sll(1, 1, 16))
    instrs.append(mips_ori(1, 1, low))
    instrs.append(mips_addi(2, 0, 0x2000))
    instrs.append(mips_addi(3, 0, 42))

    loop_start = len(instrs)
    instrs.append(mips_sw(3, 2, 0))
    instrs.append(mips_lw(4, 2, 0))
    instrs.append(mips_sw(4, 2, 4))
    instrs.append(mips_lw(5, 2, 4))
    instrs.append(mips_addi(1, 1, -1))

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(mips_bne(1, 0, offset))

    instrs.append(mips_halt())
    return instrs

def main():
    print("=" * 65)
    print(" ArchC MIPS (MIPS-I / MIPS32) Verification & Performance Suite")
    print("=" * 65)

    alu_code = gen_mips_alu(5000000)
    mem_code = gen_mips_mem(4000000)

    write_hex("mips_alu.hex", alu_code, base_addr=0x1000)
    write_hex("mips_mem.hex", mem_code, base_addr=0x1000)
    write_elf("mips_alu.elf", alu_code, base_addr=0x1000, is_elf64=True)
    write_elf("mips_mem.elf", mem_code, base_addr=0x1000, is_elf64=True)

    sim = "./mips.x"
    test_files = ["mips_alu.hex", "mips_mem.hex", "mips_alu.elf", "mips_mem.elf"]

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
