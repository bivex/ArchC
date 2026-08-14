#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (32-bit Little Endian)
def aarch64_dp_reg(op, rm, rn, rd, opt=0, imm6=0):
    return ((op & 0xFF) << 24) | ((opt & 0x7) << 21) | ((rm & 0x1F) << 16) | ((imm6 & 0x3F) << 10) | ((rn & 0x1F) << 5) | (rd & 0x1F)

def aarch64_dp_imm(op, imm14, rn, rd):
    return ((op & 0xFF) << 24) | ((imm14 & 0x3FFF) << 10) | ((rn & 0x1F) << 5) | (rd & 0x1F)

def aarch64_ls_imm(op, disp, rn, rd):
    return ((op & 0xFF) << 24) | ((disp & 0x3FFF) << 10) | ((rn & 0x1F) << 5) | (rd & 0x1F)

def aarch64_branch_cond(imm19, cond):
    return (0x54 << 24) | ((imm19 & 0x7FFFF) << 5) | (cond & 0xF)

def aarch64_halt():
    return (0x7F << 24)

arm64_halt = aarch64_halt

# Instructions
def arm64_add(rn, rm, rd):     return aarch64_dp_reg(0x8B, rm, rn, rd)
def arm64_adds(rn, rm, rd):    return aarch64_dp_reg(0xAB, rm, rn, rd)
def arm64_sub(rn, rm, rd):     return aarch64_dp_reg(0xCB, rm, rn, rd)
def arm64_subs(rn, rm, rd):    return aarch64_dp_reg(0xEB, rm, rn, rd)
def arm64_and(rn, rm, rd):     return aarch64_dp_reg(0x8A, rm, rn, rd)
def arm64_orr(rn, rm, rd):     return aarch64_dp_reg(0xAA, rm, rn, rd)
def arm64_eor(rn, rm, rd):     return aarch64_dp_reg(0xCA, rm, rn, rd)
def arm64_mul(rn, rm, rd):     return aarch64_dp_reg(0x9B, rm, rn, rd)

def arm64_add_imm(rn, imm, rd):  return aarch64_dp_imm(0x91, imm, rn, rd)
def arm64_adds_imm(rn, imm, rd): return aarch64_dp_imm(0xB1, imm, rn, rd)
def arm64_sub_imm(rn, imm, rd):  return aarch64_dp_imm(0xD1, imm, rn, rd)
def arm64_subs_imm(rn, imm, rd): return aarch64_dp_imm(0xF1, imm, rn, rd)
def arm64_movz(imm, rd):         return aarch64_dp_imm(0xD2, imm, 31, rd)

def arm64_ldr_x(rn, disp, rd):   return aarch64_ls_imm(0xF9, disp, rn, rd)
def arm64_str_x(rn, disp, rd):   return aarch64_ls_imm(0xF8, disp, rn, rd)

def arm64_b_ne(imm19):           return aarch64_branch_cond(imm19, 0x1)
def arm64_b_eq(imm19):           return aarch64_branch_cond(imm19, 0x0)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:08X} ")
        f.write("\n")

def write_elf64_aarch64(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 64
    phdr_size = 56
    file_size = len(code_bytes)

    # ELF64 Header for AArch64 (EM_AARCH64 = 183 = 0xB7)
    e_ident = b"\x7fELF\x02\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 183     # EM_AARCH64
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

def gen_aarch64_alu(outer_loops=1000, inner_loops=5000):
    # X0=acc, X1=inner_cnt, X6=outer_cnt, X2=7, X3=11, X4=13, X5=17
    code = [
        arm64_movz(outer_loops, 6), # 0: X6 = outer_loops
        arm64_movz(0, 0),           # 1: X0 = 0
        arm64_movz(7, 2),           # 2: X2 = 7
        arm64_movz(11, 3),          # 3: X3 = 11
        arm64_movz(13, 4),          # 4: X4 = 13
        arm64_movz(17, 5),          # 5: X5 = 17
    ]
    outer_start = len(code)         # 6: reload inner counter
    code.append(arm64_movz(inner_loops, 1)) # 6: X1 = inner_loops
    inner_start = len(code)         # 7
    code += [
        arm64_add(0, 2, 0),         # 7: X0 += X2
        arm64_eor(0, 3, 0),         # 8: X0 ^= X3
        arm64_add(0, 4, 0),         # 9: X0 += X4
        arm64_mul(0, 5, 0),         # 10: X0 *= X5
        arm64_add_imm(2, 1, 2),     # 11: X2 += 1
        arm64_subs_imm(1, 1, 1),    # 12: X1 -= 1 (sets Z flag)
        arm64_b_ne(inner_start - (len(code) + 6)), # 13: 7 - 13 = -6
        arm64_subs_imm(6, 1, 6),    # 14: X6 -= 1
        arm64_b_ne(outer_start - (len(code) + 8)), # 15: 6 - 15 = -9
        arm64_halt()                # 16
    ]
    return code

def gen_aarch64_mem(outer_loops=1000, inner_loops=3000):
    # X6=outer, X1=inner, X29 ($fp)=0x4000
    code = [
        arm64_movz(outer_loops, 6), # 0
        arm64_movz(0x4000, 29),     # 1: X29 = 0x4000
        arm64_movz(0x1234, 0),      # 2: X0 = 0x1234
        arm64_movz(0x5678, 2),      # 3: X2 = 0x5678
    ]
    outer_start = len(code)         # 4
    code.append(arm64_movz(inner_loops, 1)) # 4
    inner_start = len(code)         # 5
    code += [
        arm64_str_x(29, 0, 0),      # 5: [X29 + 0] = X0
        arm64_str_x(29, 8, 2),      # 6: [X29 + 8] = X2
        arm64_ldr_x(29, 0, 3),      # 7: X3 = [X29 + 0]
        arm64_ldr_x(29, 8, 4),      # 8: X4 = [X29 + 8]
        arm64_add(3, 4, 3),         # 9: X3 = X3 + X4
        arm64_add_imm(0, 1, 0),     # 10: X0 += 1
        arm64_subs_imm(1, 1, 1),    # 11: X1 -= 1
        arm64_b_ne(inner_start - (len(code) + 7)), # 12: 5 - 12 = -7
        arm64_subs_imm(6, 1, 6),    # 13: X6 -= 1
        arm64_b_ne(outer_start - (len(code) + 9)), # 14: 4 - 14 = -10
        arm64_halt()                # 15
    ]
    return code

def main():
    print("=" * 65)
    print(" ArchC AArch64 (ARM64 / ARMv8-A) Verification & Performance")
    print("=" * 65)

    test_files = [
        ("arm64_alu.hex", gen_aarch64_alu(1000, 5000), write_hex),
        ("arm64_mem.hex", gen_aarch64_mem(1000, 3000), write_hex),
        ("arm64_alu.elf", gen_aarch64_alu(1000, 5000), write_elf64_aarch64),
        ("arm64_mem.elf", gen_aarch64_mem(1000, 3000), write_elf64_aarch64),
    ]

    sim = "./aarch64.x"
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
