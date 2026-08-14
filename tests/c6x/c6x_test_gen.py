#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (32-bit Little Endian)
def c6x_vliw_reg(op, src1, src2, dst, side=0, p=0, creg=0, z=0):
    return ((creg & 0x7) << 29) | ((z & 0x1) << 28) | ((op & 0x7F) << 21) | ((src2 & 0x1F) << 16) | ((src1 & 0x1F) << 11) | ((dst & 0x1F) << 6) | ((side & 0x1) << 1) | (p & 0x1)

def c6x_vliw_imm(op, cst, dst, creg=0, z=0):
    return ((creg & 0x7) << 29) | ((z & 0x1) << 28) | ((op & 0x7F) << 21) | ((cst & 0xFFFF) << 5) | (dst & 0x1F)

def c6x_branch(disp, creg=0, z=0):
    return ((creg & 0x7) << 29) | ((z & 0x1) << 28) | (0x20 << 21) | (disp & 0x1FFFFF)

def c6x_halt():
    return (0x7F << 24)

# Instructions
def c6x_add(src1, src2, dst, side=0, p=0, creg=0, z=0):   return c6x_vliw_reg(0x01, src1, src2, dst, side, p, creg, z)
def c6x_sub(src1, src2, dst, side=0, p=0, creg=0, z=0):   return c6x_vliw_reg(0x02, src1, src2, dst, side, p, creg, z)
def c6x_xor(src1, src2, dst, side=0, p=0, creg=0, z=0):   return c6x_vliw_reg(0x05, src1, src2, dst, side, p, creg, z)
def c6x_sadd(src1, src2, dst, side=0, p=0, creg=0, z=0):  return c6x_vliw_reg(0x06, src1, src2, dst, side, p, creg, z)
def c6x_ssub(src1, src2, dst, side=0, p=0, creg=0, z=0):  return c6x_vliw_reg(0x07, src1, src2, dst, side, p, creg, z)
def c6x_mpy(src1, src2, dst, side=0, p=0, creg=0, z=0):   return c6x_vliw_reg(0x08, src1, src2, dst, side, p, creg, z)
def c6x_smpy(src1, src2, dst, side=0, p=0, creg=0, z=0):  return c6x_vliw_reg(0x09, src1, src2, dst, side, p, creg, z)
def c6x_ldw(src1, dst, side=0, p=0, creg=0, z=0):        return c6x_vliw_reg(0x0A, src1, 0, dst, side, p, creg, z)
def c6x_stw(src2, src1, side=0, p=0, creg=0, z=0):        return c6x_vliw_reg(0x0B, src1, src2, 0, side, p, creg, z)

def c6x_mvkl(cst, dst, creg=0, z=0):                      return c6x_vliw_imm(0x10, cst, dst, creg, z)
def c6x_mvkh(cst, dst, creg=0, z=0):                      return c6x_vliw_imm(0x11, cst, dst, creg, z)
def c6x_add_imm(cst, dst, creg=0, z=0):                   return c6x_vliw_imm(0x12, cst, dst, creg, z)
def c6x_sub_imm(cst, dst, creg=0, z=0):                   return c6x_vliw_imm(0x13, cst, dst, creg, z)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:08X} ")
        f.write("\n")

def write_elf_c6x(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for TI C6000 (EM_TI_C6000 = 140 = 0x8C)
    e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 140     # EM_TI_C6000
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

    ehdr = struct.pack("<16sHHIIIIIHHHHHH",
        e_ident, e_type, e_machine, e_version, e_entry,
        e_phoff, e_shoff, e_flags, e_ehsize, e_phentsize, e_phnum,
        e_shentsize, e_shnum, e_shstrndx
    )

    offset = ehdr_size + phdr_size
    p_type = 1          # PT_LOAD
    p_offset = offset
    p_vaddr = entry
    p_paddr = entry
    p_filesz = file_size
    p_memsz = file_size + 0x1000
    p_flags = 7         # PF_R | PF_W | PF_X
    p_align = 4

    phdr = struct.pack("<IIIIIIII",
        p_type, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_flags, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_c6x_dsp(outer_loops=1000, inner_loops=5000):
    # A1 = outer counter (creg=4), A2 = inner counter (creg=5)
    # A0 = accumulator, A3 = 7, A4 = 11, A5 = 13, A6 = 17
    code = [
        c6x_mvkl(outer_loops, 1), # 0: A1 = outer_loops
        c6x_mvkl(0, 0),           # 1: A0 = 0
        c6x_mvkl(7, 3),           # 2: A3 = 7
        c6x_mvkl(11, 4),          # 3: A4 = 11
        c6x_mvkl(13, 5),          # 4: A5 = 13
        c6x_mvkl(17, 6),          # 5: A6 = 17
    ]
    outer_start = len(code)       # 6
    code.append(c6x_mvkl(inner_loops, 2)) # 6: A2 = inner_loops
    inner_start = len(code)       # 7
    code += [
        c6x_sadd(0, 3, 0),        # 7: A0 = sadd(A0, A3)
        c6x_xor(0, 4, 0),         # 8: A0 ^= A4
        c6x_sadd(0, 5, 0),        # 9: A0 = sadd(A0, A5)
        c6x_smpy(0, 6, 0),        # 10: A0 = smpy(A0, A6)
        c6x_add_imm(1, 3),        # 11: A3 += 1
        c6x_sub_imm(1, 2),        # 12: A2 -= 1
        # [A2] b inner_start (creg=5: test A2 != 0)
        c6x_branch(inner_start - (len(code) + 6), creg=5, z=0), # 13: 7 - 13 = -6
        c6x_sub_imm(1, 1),        # 14: A1 -= 1
        # [A1] b outer_start (creg=4: test A1 != 0)
        c6x_branch(outer_start - (len(code) + 8), creg=4, z=0), # 15: 6 - 15 = -9
        c6x_halt()                # 16
    ]
    return code

def gen_c6x_mem(outer_loops=1000, inner_loops=3000):
    # A1 = outer counter, A2 = inner counter, A15 = $sp = 0x4000
    code = [
        c6x_mvkl(outer_loops, 1), # 0
        c6x_mvkl(0x4000, 15),     # 1: A15 = 0x4000
        c6x_mvkl(0x1234, 0),      # 2: A0 = 0x1234
        c6x_mvkl(0x5678, 3),      # 3: A3 = 0x5678
    ]
    outer_start = len(code)       # 4
    code.append(c6x_mvkl(inner_loops, 2)) # 4
    inner_start = len(code)       # 5
    code += [
        c6x_stw(0, 15),           # 5: *A15 = A0
        c6x_ldw(15, 4),           # 6: A4 = *A15
        c6x_sadd(4, 3, 4),        # 7: A4 = sadd(A4, A3)
        c6x_add_imm(1, 0),        # 8: A0 += 1
        c6x_sub_imm(1, 2),        # 9: A2 -= 1
        c6x_branch(inner_start - (len(code) + 5), creg=5, z=0), # 10: 5 - 10 = -5
        c6x_sub_imm(1, 1),        # 11: A1 -= 1
        c6x_branch(outer_start - (len(code) + 7), creg=4, z=0), # 12: 4 - 12 = -8
        c6x_halt()                # 13
    ]
    return code

def main():
    print("=" * 65)
    print(" ArchC TI TMS320C6000 (C6x VLIW DSP) Verification & Performance")
    print("=" * 65)

    test_files = [
        ("c6x_dsp.hex", gen_c6x_dsp(1000, 5000), write_hex),
        ("c6x_mem.hex", gen_c6x_mem(1000, 3000), write_hex),
        ("c6x_dsp.elf", gen_c6x_dsp(1000, 5000), write_elf_c6x),
        ("c6x_mem.elf", gen_c6x_mem(1000, 3000), write_elf_c6x),
    ]

    sim = "./c6x.x"
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
