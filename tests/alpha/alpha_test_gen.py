#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (32-bit Little Endian)
def alpha_op_reg(op, ra, rb, sbz, func, rc):
    return ((op & 0x3F) << 26) | ((ra & 0x1F) << 21) | ((rb & 0x1F) << 16) | ((sbz & 0xF) << 12) | ((func & 0x7F) << 5) | (rc & 0x1F)

def alpha_op_lit(op, ra, lit, flag, func, rc):
    return ((op & 0x3F) << 26) | ((ra & 0x1F) << 21) | ((lit & 0xFF) << 13) | ((flag & 0x1) << 12) | ((func & 0x7F) << 5) | (rc & 0x1F)

def alpha_memory(op, ra, rb, disp):
    return ((op & 0x3F) << 26) | ((ra & 0x1F) << 21) | ((rb & 0x1F) << 16) | (disp & 0xFFFF)

def alpha_branch(op, ra, bdisp):
    return ((op & 0x3F) << 26) | ((ra & 0x1F) << 21) | (bdisp & 0x1FFFFF)

def alpha_halt():
    return 0x00000000

# Instructions
def alpha_addq(ra, rb, rc):   return alpha_op_reg(0x10, ra, rb, 0, 0x20, rc)
def alpha_subq(ra, rb, rc):   return alpha_op_reg(0x10, ra, rb, 0, 0x29, rc)
def alpha_mulq(ra, rb, rc):   return alpha_op_reg(0x13, ra, rb, 0, 0x20, rc)
def alpha_cmpeq(ra, rb, rc):  return alpha_op_reg(0x10, ra, rb, 0, 0x2D, rc)
def alpha_cmplt(ra, rb, rc):  return alpha_op_reg(0x10, ra, rb, 0, 0x4D, rc)
def alpha_cmple(ra, rb, rc):  return alpha_op_reg(0x10, ra, rb, 0, 0x6D, rc)
def alpha_and(ra, rb, rc):    return alpha_op_reg(0x11, ra, rb, 0, 0x00, rc)
def alpha_bis(ra, rb, rc):    return alpha_op_reg(0x11, ra, rb, 0, 0x20, rc)
def alpha_xor(ra, rb, rc):    return alpha_op_reg(0x11, ra, rb, 0, 0x40, rc)
def alpha_cmoveq(ra, rb, rc): return alpha_op_reg(0x11, ra, rb, 0, 0x24, rc)
def alpha_cmovne(ra, rb, rc): return alpha_op_reg(0x11, ra, rb, 0, 0x26, rc)

def alpha_addq_i(ra, lit, rc): return alpha_op_lit(0x10, ra, lit, 1, 0x20, rc)
def alpha_subq_i(ra, lit, rc): return alpha_op_lit(0x10, ra, lit, 1, 0x29, rc)

def alpha_lda(ra, rb, disp):  return alpha_memory(0x08, ra, rb, disp)
def alpha_ldah(ra, rb, disp): return alpha_memory(0x09, ra, rb, disp)
def alpha_ldq(ra, rb, disp):  return alpha_memory(0x29, ra, rb, disp)
def alpha_stq(ra, rb, disp):  return alpha_memory(0x2D, ra, rb, disp)
def alpha_ldl(ra, rb, disp):  return alpha_memory(0x28, ra, rb, disp)
def alpha_stl(ra, rb, disp):  return alpha_memory(0x2C, ra, rb, disp)

def alpha_bne(ra, bdisp):     return alpha_branch(0x3D, ra, bdisp)
def alpha_beq(ra, bdisp):     return alpha_branch(0x39, ra, bdisp)
def alpha_br(ra, bdisp):      return alpha_branch(0x30, ra, bdisp)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:08X} ")
        f.write("\n")

def write_elf64_alpha(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 64
    phdr_size = 56
    file_size = len(code_bytes)

    # ELF64 Header for DEC Alpha (EM_ALPHA = 0x9026 = 36902)
    e_ident = b"\x7fELF\x02\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 0x9026  # EM_ALPHA
    e_version = 1
    e_entry = entry
    e_phoff = ehdr_size
    e_shoff = 0
    e_flags = 0
    e_ehsize = ehdr_size
    e_phentsize = phdr_size
    e_phnum = 1
    e_shentsize = 0
    e_shnum = 0
    e_shstrndx = 0

    ehdr = struct.pack("<16sHHIQQQIHHHHHH",
        e_ident, e_type, e_machine, e_version, e_entry,
        e_phoff, e_shoff, e_flags, e_ehsize, e_phentsize, e_phnum,
        e_shentsize, e_shnum, e_shstrndx
    )

    offset = ehdr_size + phdr_size
    p_type = 1          # PT_LOAD
    p_flags = 7         # PF_R | PF_W | PF_X
    p_offset = offset
    p_vaddr = entry
    p_paddr = entry
    p_filesz = file_size
    p_memsz = file_size + 0x1000
    p_align = 8

    phdr = struct.pack("<IIQQQQQQ",
        p_type, p_flags, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_alpha_alu(outer_loops=1000, inner_loops=5000):
    # Register map: r6=outer_counter, r1=inner_counter, r0=0, r2=7, r3=11, r4=13, r5=17
    code = [
        alpha_lda(6, 31, outer_loops), # 0: r6 = outer_loops
        alpha_lda(0, 31, 0),           # 1: r0 = 0
        alpha_lda(2, 31, 7),           # 2: r2 = 7
        alpha_lda(3, 31, 11),          # 3: r3 = 11
        alpha_lda(4, 31, 13),          # 4: r4 = 13
        alpha_lda(5, 31, 17),          # 5: r5 = 17
    ]
    outer_start = len(code)            # 6: reload inner loop counter
    code.append(alpha_lda(1, 31, inner_loops))
    inner_start = len(code)            # 7: inner loop start
    code += [
        alpha_addq(0, 2, 0),           # 7: r0 += r2
        alpha_xor(0, 3, 0),            # 8: r0 ^= r3
        alpha_addq(0, 4, 0),           # 9: r0 += r4
        alpha_mulq(0, 5, 0),           # 10: r0 *= r5
        alpha_addq_i(2, 1, 2),         # 11: r2 += 1
        alpha_subq_i(1, 1, 1),         # 12: r1 -= 1
        alpha_bne(1, inner_start - (len(code) + 6)), # 13: bne r1, inner_start (offset = 7 - 13 = -6)
        alpha_subq_i(6, 1, 6),         # 14: r6 -= 1
        alpha_bne(6, outer_start - (len(code) + 8)), # 15: bne r6, outer_start (offset = 6 - 15 = -9)
        alpha_halt()                   # 16
    ]
    return code

def gen_alpha_mem(outer_loops=1000, inner_loops=3000):
    code = [
        alpha_lda(6, 31, outer_loops), # 0: r6 = outer_loops
        alpha_lda(30, 31, 0x4000),     # 1: $sp = 0x4000
        alpha_lda(0, 31, 0x1234),      # 2: r0 = 0x1234
        alpha_lda(2, 31, 0x5678),      # 3: r2 = 0x5678
    ]
    outer_start = len(code)            # 4
    code.append(alpha_lda(1, 31, inner_loops))
    inner_start = len(code)            # 5
    code += [
        alpha_stq(0, 30, 0),           # 5
        alpha_stq(2, 30, 8),           # 6
        alpha_ldq(3, 30, 0),           # 7
        alpha_ldq(4, 30, 8),           # 8
        alpha_addq(3, 4, 3),           # 9
        alpha_addq_i(0, 1, 0),         # 10
        alpha_subq_i(1, 1, 1),         # 11
        alpha_bne(1, inner_start - (len(code) + 7)), # 12: 5 - 12 = -7
        alpha_subq_i(6, 1, 6),         # 13
        alpha_bne(6, outer_start - (len(code) + 9)), # 14: 4 - 14 = -10
        alpha_halt()                   # 15
    ]
    return code

def main():
    print("=" * 65)
    print(" ArchC DEC Alpha (Alpha 21264 / AXP) Verification & Performance")
    print("=" * 65)

    test_files = [
        ("alpha_alu.hex", gen_alpha_alu(1000, 5000), write_hex),
        ("alpha_mem.hex", gen_alpha_mem(1000, 3000), write_hex),
        ("alpha_alu.elf", gen_alpha_alu(1000, 5000), write_elf64_alpha),
        ("alpha_mem.elf", gen_alpha_mem(1000, 3000), write_elf64_alpha),
    ]

    sim = "./alpha.x"
    for tf, code, writer in test_files:
        writer(tf, code)
        p = subprocess.run([sim, f"--load={tf}"], capture_output=True, text=True)
        stats = {}
        out = p.stdout + "\n" + p.stderr
        for line in out.splitlines():
            if "Number of instructions executed:" in line or "Simulation speed:" in line:
                parts = line.split(":", 1)
                stats[parts[0].strip()] = parts[1].strip()

        inst_count = stats.get("Number of instructions executed", "N/A")
        speed = stats.get("Simulation speed", "N/A")
        print(f"  {tf:<15} | Number of instructions executed: {inst_count:<12} | Simulation speed: {speed}")

if __name__ == "__main__":
    main()
