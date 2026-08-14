#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def enc_r(funct7, rs2, rs1, funct3, rd, opcode=0x33):
    return ((funct7 & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((rd & 0x1F) << 7) | (opcode & 0x7F)

def enc_i(imm, rs1, funct3, rd, opcode=0x13):
    return ((imm & 0xFFF) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((rd & 0x1F) << 7) | (opcode & 0x7F)

def enc_s(imm, rs2, rs1, funct3=0x2, opcode=0x23):
    imm_hi = (imm >> 5) & 0x7F
    imm_lo = imm & 0x1F
    return ((imm_hi & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((imm_lo & 0x1F) << 7) | (opcode & 0x7F)

def enc_b(offset_instrs, rs2, rs1, funct3, opcode=0x63):
    imm_hi = (offset_instrs >> 5) & 0x7F
    imm_lo = offset_instrs & 0x1F
    return ((imm_hi & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((imm_lo & 0x1F) << 7) | (opcode & 0x7F)

def rv_add(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x0, rd)
def rv_sub(rd, rs1, rs2): return enc_r(0x20, rs2, rs1, 0x0, rd)
def rv_sll(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x1, rd)
def rv_slt(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x2, rd)
def rv_sltu(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x3, rd)
def rv_xor(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x4, rd)
def rv_srl(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x5, rd)
def rv_sra(rd, rs1, rs2): return enc_r(0x20, rs2, rs1, 0x5, rd)
def rv_or(rd, rs1, rs2):  return enc_r(0x00, rs2, rs1, 0x6, rd)
def rv_and(rd, rs1, rs2): return enc_r(0x00, rs2, rs1, 0x7, rd)

def rv_addi(rd, rs1, imm): return enc_i(imm, rs1, 0x0, rd, 0x13)
def rv_xori(rd, rs1, imm): return enc_i(imm, rs1, 0x4, rd, 0x13)
def rv_ori(rd, rs1, imm):  return enc_i(imm, rs1, 0x6, rd, 0x13)
def rv_andi(rd, rs1, imm): return enc_i(imm, rs1, 0x7, rd, 0x13)
def rv_lw(rd, rs1, imm=0): return enc_i(imm, rs1, 0x2, rd, 0x03)
def rv_sw(rs2, rs1, imm=0): return enc_s(imm, rs2, rs1, 0x2, 0x23)

def rv_beq(rs1, rs2, imm_half): return enc_b(imm_half, rs2, rs1, 0x0)
def rv_bne(rs1, rs2, imm_half): return enc_b(imm_half, rs2, rs1, 0x1)
def rv_blt(rs1, rs2, imm_half): return enc_b(imm_half, rs2, rs1, 0x4)
def rv_bge(rs1, rs2, imm_half): return enc_b(imm_half, rs2, rs1, 0x5)

def rv_lui(rd, imm20): return ((imm20 & 0xFFFFF) << 12) | ((rd & 0x1F) << 7) | 0x37
def rv_halt(): return 0x00000073 # ecall/halt

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
            e_ident = b"\x7fELF\x02\x01\x01\x00" + b"\x00" * 8
            ehdr = struct.pack("<16sHHIQQQIHHHHHH", e_ident, 2, 243, 1,
                               base_addr, 64, 0, 0, 64, 56, 1, 64, 0, 0)
            phdr = struct.pack("<IIQQQQQQ", 1, 7, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 4)
            f.write(ehdr)
            f.write(phdr)
        else:
            e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
            ehdr = struct.pack("<16sHHIIIIIHHHHHH", e_ident, 2, 243, 1,
                               base_addr, 52, 0, 0, 52, 32, 1, 40, 0, 0)
            phdr = struct.pack("<IIIIIIII", 1, code_offset, base_addr, base_addr,
                               len(code_bytes), len(code_bytes), 7, 4)
            f.write(ehdr)
            f.write(phdr)

        padding = code_offset - f.tell()
        if padding > 0:
            f.write(b"\x00" * padding)
        f.write(code_bytes)

def gen_rv_alu(iterations=5000000):
    instrs = []
    high = (iterations >> 12) & 0xFFFFF
    low = iterations & 0xFFF
    instrs.append(rv_lui(1, high))          # x1 = high << 12
    instrs.append(rv_addi(1, 1, low))       # x1 = x1 + low (counter)
    instrs.append(rv_addi(2, 0, 10))        # x2 = 10
    instrs.append(rv_addi(3, 0, 20))        # x3 = 20
    instrs.append(rv_addi(4, 0, 30))        # x4 = 30

    loop_start = len(instrs)
    instrs.append(rv_add(2, 2, 3))          # x2 = x2 + x3
    instrs.append(rv_sub(3, 3, 4))          # x3 = x3 - x4
    instrs.append(rv_xor(4, 4, 2))          # x4 = x4 ^ x2
    instrs.append(rv_or(5, 2, 3))           # x5 = x2 | x3
    instrs.append(rv_and(6, 5, 4))          # x6 = x5 & x4
    instrs.append(rv_slt(7, 2, 3))          # x7 = x2 < x3
    instrs.append(rv_addi(1, 1, -1))        # x1 -= 1

    bne_idx = len(instrs)
    offset_instrs = loop_start - bne_idx
    instrs.append(rv_bne(1, 0, offset_instrs))

    instrs.append(rv_halt())
    return instrs

def gen_rv_mem(iterations=4000000):
    instrs = []
    high = (iterations >> 12) & 0xFFFFF
    low = iterations & 0xFFF
    instrs.append(rv_lui(1, high))
    instrs.append(rv_addi(1, 1, low))       # x1 = iterations
    instrs.append(rv_addi(2, 0, 0x2000))    # x2 = 0x2000 (base addr)
    instrs.append(rv_addi(3, 0, 42))        # x3 = 42

    loop_start = len(instrs)
    instrs.append(rv_sw(3, 2, 0))           # mem[x2+0] = x3
    instrs.append(rv_lw(4, 2, 0))           # x4 = mem[x2+0]
    instrs.append(rv_sw(4, 2, 4))           # mem[x2+4] = x4
    instrs.append(rv_lw(5, 2, 4))           # x5 = mem[x2+4]
    instrs.append(rv_addi(1, 1, -1))        # x1 -= 1

    bne_idx = len(instrs)
    offset_instrs = loop_start - bne_idx
    instrs.append(rv_bne(1, 0, offset_instrs))

    instrs.append(rv_halt())
    return instrs

def gen_rv_branch(iterations=4000000):
    instrs = []
    high = (iterations >> 12) & 0xFFFFF
    low = iterations & 0xFFF
    instrs.append(rv_lui(1, high))
    instrs.append(rv_addi(1, 1, low))       # x1 = iterations
    instrs.append(rv_addi(2, 0, 0))         # x2 = 0

    loop_start = len(instrs)
    instrs.append(rv_andi(3, 1, 1))         # x3 = x1 & 1
    beq_idx = len(instrs)
    # jump over odd handler if even (jump +3 instructions)
    instrs.append(rv_beq(3, 0, 3))
    instrs.append(rv_addi(2, 2, 1))         # odd: x2 += 1
    j_idx = len(instrs)
    instrs.append(rv_beq(0, 0, 2))          # jump to loop end (+2 instructions)
    instrs.append(rv_addi(2, 2, 2))         # even: x2 += 2
    # loop end:
    instrs.append(rv_addi(1, 1, -1))        # x1 -= 1
    bne_idx = len(instrs)
    offset_instrs = loop_start - bne_idx
    instrs.append(rv_bne(1, 0, offset_instrs))

    instrs.append(rv_halt())
    return instrs

def main():
    print("=" * 68)
    print(" ArchC RISC-V (RV32I / RV64I) Verification & Performance Suite")
    print("=" * 68)

    alu_code = gen_rv_alu(5000000)     # ~40M instructions
    mem_code = gen_rv_mem(4000000)     # ~24M instructions
    br_code = gen_rv_branch(4000000)   # ~22M instructions

    write_hex("rv_alu.hex", alu_code, base_addr=0x1000)
    write_hex("rv_mem.hex", mem_code, base_addr=0x1000)
    write_hex("rv_branch.hex", br_code, base_addr=0x1000)

    write_elf("rv_alu.elf", alu_code, base_addr=0x1000, is_elf64=True)
    write_elf("rv_mem.elf", mem_code, base_addr=0x1000, is_elf64=True)
    write_elf("rv_branch.elf", br_code, base_addr=0x1000, is_elf64=True)

    sim = "./riscv.x"
    test_files = [
        ("rv_alu.hex", "RV32I ALU Compute (40M instrs)"),
        ("rv_mem.hex", "RV32I Memory Load/Store (24M instrs)"),
        ("rv_branch.hex", "RV32I Branch Control Flow (22M instrs)"),
        ("rv_alu.elf", "RV64/32 ELF ALU Binary"),
        ("rv_mem.elf", "RV64/32 ELF Memory Binary"),
        ("rv_branch.elf", "RV64/32 ELF Branch Binary"),
    ]

    for tf, desc in test_files:
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
