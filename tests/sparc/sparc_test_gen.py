#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def enc_f2(op, rd, op2, imm22):
    return ((op & 0x3) << 30) | ((rd & 0x1F) << 25) | ((op2 & 0x7) << 22) | (imm22 & 0x3FFFFF)

def enc_f3i(op, rd, op3, rs1, simm13):
    return ((op & 0x3) << 30) | ((rd & 0x1F) << 25) | ((op3 & 0x3F) << 19) | ((rs1 & 0x1F) << 14) | (1 << 13) | (simm13 & 0x1FFF)

def enc_f3r(op, rd, op3, rs1, rs2):
    return ((op & 0x3) << 30) | ((rd & 0x1F) << 25) | ((op3 & 0x3F) << 19) | ((rs1 & 0x1F) << 14) | (0 << 13) | (rs2 & 0x1F)

def enc_br(cond, disp22):
    return (0 << 30) | (0 << 29) | ((cond & 0xF) << 25) | (2 << 22) | (disp22 & 0x3FFFFF)

def sparc_add_r(rd, rs1, rs2): return enc_f3r(2, rd, 0x00, rs1, rs2)
def sparc_add_i(rd, rs1, simm13): return enc_f3i(2, rd, 0x00, rs1, simm13)
def sparc_sub_r(rd, rs1, rs2): return enc_f3r(2, rd, 0x04, rs1, rs2)
def sparc_sub_i(rd, rs1, simm13): return enc_f3i(2, rd, 0x04, rs1, simm13)
def sparc_and_r(rd, rs1, rs2): return enc_f3r(2, rd, 0x01, rs1, rs2)
def sparc_or_r(rd, rs1, rs2):  return enc_f3r(2, rd, 0x02, rs1, rs2)
def sparc_xor_r(rd, rs1, rs2): return enc_f3r(2, rd, 0x03, rs1, rs2)
def sparc_sll_i(rd, rs1, simm13): return enc_f3i(2, rd, 0x25, rs1, simm13)
def sparc_sethi(rd, imm22): return enc_f2(0, rd, 4, imm22)
def sparc_ld(rd, rs1, simm13=0): return enc_f3i(3, rd, 0x00, rs1, simm13)
def sparc_st(rd, rs1, simm13=0): return enc_f3i(3, rd, 0x04, rs1, simm13)
def sparc_bne(disp22): return enc_br(9, disp22)
def sparc_halt(): return (3 << 30) | (0x3F << 19)

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
            e_ident = b"\x7fELF\x02\x02\x01\x00" + b"\x00" * 8 # SPARC V9/V8 64-bit BE
            ehdr = struct.pack(">16sHHIQQQIHHHHHH", e_ident, 2, 43, 1,
                               base_addr, 64, 0, 0, 64, 56, 1, 64, 0, 0)
            phdr = struct.pack(">IIQQQQQQ", 1, 7, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 4)
            f.write(ehdr)
            f.write(phdr)
        else:
            e_ident = b"\x7fELF\x01\x02\x01\x00" + b"\x00" * 8 # SPARC V8 32-bit BE
            ehdr = struct.pack(">16sHHIIIIIHHHHHH", e_ident, 2, 2, 1,
                               base_addr, 52, 0, 0, 52, 32, 1, 40, 0, 0)
            phdr = struct.pack(">IIIIIIII", 1, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 7, 4)
            f.write(ehdr)
            f.write(phdr)

        padding = code_offset - f.tell()
        if padding > 0:
            f.write(b"\x00" * padding)
        f.write(code_bytes)

def gen_sparc_alu(iterations=5000000):
    instrs = []
    high = (iterations >> 10) & 0x3FFFFF
    low = iterations & 0x3FF
    instrs.append(sparc_sethi(1, high))         # %g1 = high << 10
    instrs.append(sparc_add_i(1, 1, low))       # %g1 = %g1 + low
    instrs.append(sparc_add_i(2, 0, 10))        # %g2 = 10
    instrs.append(sparc_add_i(3, 0, 20))        # %g3 = 20
    instrs.append(sparc_add_i(4, 0, 30))        # %g4 = 30

    loop_start = len(instrs)
    instrs.append(sparc_add_r(2, 2, 3))         # %g2 = %g2 + %g3
    instrs.append(sparc_sub_r(3, 3, 4))         # %g3 = %g3 - %g4
    instrs.append(sparc_xor_r(4, 4, 2))         # %g4 = %g4 ^ %g2
    instrs.append(sparc_or_r(5, 2, 3))          # %g5 = %g2 | %g3
    instrs.append(sparc_and_r(6, 5, 4))         # %g6 = %g5 & %g4
    instrs.append(sparc_sub_i(1, 1, 1))         # %g1 = %g1 - 1 (sets Z flag)

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(sparc_bne(offset))

    instrs.append(sparc_halt())
    return instrs

def gen_sparc_mem(iterations=4000000):
    instrs = []
    high = (iterations >> 10) & 0x3FFFFF
    low = iterations & 0x3FF
    instrs.append(sparc_sethi(1, high))
    instrs.append(sparc_add_i(1, 1, low))
    instrs.append(sparc_add_i(2, 0, 0x2000))    # %g2 = 0x2000
    instrs.append(sparc_add_i(3, 0, 42))        # %g3 = 42

    loop_start = len(instrs)
    instrs.append(sparc_st(3, 2, 0))            # mem[%g2+0] = %g3
    instrs.append(sparc_ld(4, 2, 0))            # %g4 = mem[%g2+0]
    instrs.append(sparc_st(4, 2, 4))            # mem[%g2+4] = %g4
    instrs.append(sparc_ld(5, 2, 4))            # %g5 = mem[%g2+4]
    instrs.append(sparc_sub_i(1, 1, 1))

    bne_idx = len(instrs)
    offset = loop_start - bne_idx
    instrs.append(sparc_bne(offset))

    instrs.append(sparc_halt())
    return instrs

def main():
    print("=" * 65)
    print(" ArchC SPARC (SPARC V8) Verification & Performance Suite")
    print("=" * 65)

    alu_code = gen_sparc_alu(5000000)
    mem_code = gen_sparc_mem(4000000)

    write_hex("sparc_alu.hex", alu_code, base_addr=0x1000)
    write_hex("sparc_mem.hex", mem_code, base_addr=0x1000)
    write_elf("sparc_alu.elf", alu_code, base_addr=0x1000, is_elf64=True)
    write_elf("sparc_mem.elf", mem_code, base_addr=0x1000, is_elf64=True)

    sim = "./sparc.x"
    test_files = ["sparc_alu.hex", "sparc_mem.hex", "sparc_alu.elf", "sparc_mem.elf"]

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
        print(f"  {tf:<16} | {instr_line:<43} | {speed_line}")

if __name__ == "__main__":
    main()
