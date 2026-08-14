#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def arm64_dp_reg(op, rm, rn, rd, opt=0, subop=0):
    return ((op & 0xFF) << 24) | ((opt & 0x7) << 21) | ((rm & 0x1F) << 16) | ((subop & 0x3F) << 10) | ((rn & 0x1F) << 5) | (rd & 0x1F)

def arm64_dp_imm(op, imm14, rn, rd):
    return ((op & 0xFF) << 24) | ((imm14 & 0x3FFF) << 10) | ((rn & 0x1F) << 5) | (rd & 0x1F)

def arm64_ls_imm(op, disp, rn, rd):
    return ((op & 0xFF) << 24) | ((disp & 0x3FFF) << 10) | ((rn & 0x1F) << 5) | (rd & 0x1F)

def arm64_pac(op, pac_op, rn, rd, subop=0):
    return ((op & 0xFF) << 24) | ((pac_op & 0xFF) << 16) | ((rn & 0x1F) << 11) | ((subop & 0x3F) << 5) | (rd & 0x1F)

def arm64_branch_cond(imm19, cond):
    return (0x54 << 24) | ((imm19 & 0x7FFFF) << 5) | (cond & 0xF)

def arm64_halt():
    return (0x7F << 24)

# Instruction helper functions
def apple_add(rn, rm, rd):     return arm64_dp_reg(0x8B, rm, rn, rd)
def apple_sub(rn, rm, rd):     return arm64_dp_reg(0xCB, rm, rn, rd)
def apple_and(rn, rm, rd):     return arm64_dp_reg(0x8A, rm, rn, rd)
def apple_orr(rn, rm, rd):     return arm64_dp_reg(0xAA, rm, rn, rd)
def apple_eor(rn, rm, rd):     return arm64_dp_reg(0xCA, rm, rn, rd)
def apple_mul(rn, rm, rd):     return arm64_dp_reg(0x9B, rm, rn, rd)
def apple_amx(rn, rm, rd):     return arm64_dp_reg(0x7E, rm, rn, rd)

def apple_add_imm(rn, imm, rd):  return arm64_dp_imm(0x91, imm, rn, rd)
def apple_subs_imm(rn, imm, rd): return arm64_dp_imm(0xF1, imm, rn, rd)
def apple_movz(imm, rd):         return arm64_dp_imm(0xD2, imm, 31, rd)

def apple_ldr_x(rn, disp, rd):   return arm64_ls_imm(0xF9, disp, rn, rd)
def apple_str_x(rn, disp, rd):   return arm64_ls_imm(0xF8, disp, rn, rd)

def apple_pacia(rn, rd):         return arm64_pac(0xDA, 0x01, rn, rd)
def apple_autia(rn, rd):         return arm64_pac(0xDA, 0x05, rn, rd)
def apple_pacda(rn, rd):         return arm64_pac(0xDA, 0x02, rn, rd)
def apple_autda(rn, rd):         return arm64_pac(0xDA, 0x06, rn, rd)

def apple_b_ne(imm19):           return arm64_branch_cond(imm19, 0x1)
def apple_b_eq(imm19):           return arm64_branch_cond(imm19, 0x0)

def write_elf64_apple_arm(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 64
    phdr_size = 56
    file_size = len(code_bytes)

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
    p_memsz = file_size + 0x10000
    p_align = 8

    phdr = struct.pack("<IIQQQQQQ",
        p_type, p_flags, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_alu_stress(outer_loops=1000, inner_loops=5000):
    # Total instructions executed: ~35M
    code = [
        apple_movz(outer_loops, 6),
        apple_movz(0, 0),
        apple_movz(7, 2),
        apple_movz(11, 3),
        apple_movz(13, 4),
        apple_movz(17, 5),
    ]
    outer_start = len(code)
    code.append(apple_movz(inner_loops, 1))
    inner_start = len(code)
    code += [
        apple_add(0, 2, 0),
        apple_eor(0, 3, 0),
        apple_add(0, 4, 0),
        apple_mul(0, 5, 0),
        apple_add_imm(2, 1, 2),
        apple_subs_imm(1, 1, 1),
        apple_b_ne(inner_start - (len(code) + 6)),
        apple_subs_imm(6, 1, 6),
        apple_b_ne(outer_start - (len(code) + 8)),
        arm64_halt()
    ]
    return code

def gen_pac_stress(outer_loops=500, inner_loops=4000):
    # Apple PAC Security (~12M instrs)
    code = [
        apple_movz(outer_loops, 6), # 0
        apple_movz(0x1000, 2),      # 1
        apple_movz(0x7FFF, 3),      # 2
    ]
    outer_start = len(code)         # 3
    code.append(apple_movz(inner_loops, 1)) # 3
    inner_start = len(code)         # 4
    code += [
        apple_pacia(3, 2),          # 4
        apple_autia(3, 2),          # 5
        apple_pacda(3, 2),          # 6
        apple_autda(3, 2),          # 7
        apple_subs_imm(1, 1, 1),     # 8
        apple_b_ne(4 - 9),          # 9: -5
        apple_subs_imm(6, 1, 6),     # 10
        apple_b_ne(3 - 11),         # 11: -8
        arm64_halt()                # 12
    ]
    return code

def gen_amx_stress(outer_loops=1000, inner_loops=3000):
    # Apple AMX Matrix Coprocessor (~18M instrs)
    code = [
        apple_movz(outer_loops, 6), # 0
        apple_movz(15, 2),          # 1
        apple_movz(25, 3),          # 2
        apple_movz(0, 4),           # 3
    ]
    outer_start = len(code)         # 4
    code.append(apple_movz(inner_loops, 1)) # 4
    inner_start = len(code)         # 5
    code += [
        apple_amx(2, 3, 4),         # 5
        apple_amx(3, 2, 4),         # 6
        apple_amx(2, 3, 4),         # 7
        apple_amx(3, 2, 4),         # 8
        apple_subs_imm(1, 1, 1),     # 9
        apple_b_ne(5 - 10),         # 10: -5
        apple_subs_imm(6, 1, 6),     # 11
        apple_b_ne(4 - 12),         # 12: -8
        arm64_halt()                # 13
    ]
    return code

def gen_mem_stress(outer_loops=1000, inner_loops=3000):
    # 64-bit Memory Load/Store (~24M instrs)
    code = [
        apple_movz(outer_loops, 6), # 0
        apple_movz(0x4000, 29),     # 1
        apple_movz(0x1234, 0),      # 2
        apple_movz(0x5678, 2),      # 3
    ]
    outer_start = len(code)         # 4
    code.append(apple_movz(inner_loops, 1)) # 4
    inner_start = len(code)         # 5
    code += [
        apple_str_x(29, 0, 0),      # 5
        apple_str_x(29, 8, 2),      # 6
        apple_ldr_x(29, 0, 3),      # 7
        apple_ldr_x(29, 8, 4),      # 8
        apple_add(3, 4, 3),         # 9
        apple_add_imm(0, 1, 0),     # 10
        apple_subs_imm(1, 1, 1),     # 11
        apple_b_ne(5 - 12),         # 12: -7
        apple_subs_imm(6, 1, 6),     # 13
        apple_b_ne(4 - 14),         # 14: -10
        arm64_halt()                # 15
    ]
    return code

def run_test(name, elf_file):
    cmd = ["./apple_arm.x", "--load=" + elf_file]
    res = subprocess.run(cmd, capture_output=True, text=True)
    out = res.stdout + "\n" + res.stderr
    
    inst_count = "N/A"
    speed = "N/A"
    for line in out.splitlines():
        if "Number of instructions executed:" in line:
            inst_count = line.split(":")[-1].strip()
        if "Simulation speed:" in line:
            speed = line.split(":")[-1].strip()
            
    print(f"  {name:<22} | Number of instructions executed: {inst_count:<12} | Simulation speed: {speed}")
    return res.returncode == 0

if __name__ == "__main__":
    print("=" * 70)
    print(" ArchC Apple Silicon ARM64e (M1/M2/M3/M4 + PAC + AMX) Suite")
    print("=" * 70)
    
    write_elf64_apple_arm("apple_arm_alu.elf", gen_alu_stress(1000, 5000))
    write_elf64_apple_arm("apple_arm_pac.elf", gen_pac_stress(500, 4000))
    write_elf64_apple_arm("apple_arm_amx.elf", gen_amx_stress(1000, 3000))
    write_elf64_apple_arm("apple_arm_mem.elf", gen_mem_stress(1000, 3000))
    
    run_test("apple_arm_alu.elf", "apple_arm_alu.elf")
    run_test("apple_arm_pac.elf", "apple_arm_pac.elf")
    run_test("apple_arm_amx.elf", "apple_arm_amx.elf")
    run_test("apple_arm_mem.elf", "apple_arm_mem.elf")
