#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (16-bit Big-Endian)
def m68k_moveq(data, rd):
    return (0x7 << 12) | ((rd & 0x7) << 9) | (data & 0xFF)

def m68k_alu(op, rd, reg):
    return ((op & 0xF) << 12) | ((rd & 0x7) << 9) | (0x2 << 6) | (reg & 0x7)

def m68k_mul(rd, reg):
    return (0xC << 12) | ((rd & 0x7) << 9) | (0x7 << 6) | (reg & 0x7)

def m68k_addq(qdata, reg):
    return (0x5 << 12) | ((qdata & 0x7) << 9) | (0x0 << 8) | (0x2 << 6) | (reg & 0x7)

def m68k_subq(qdata, reg):
    return (0x5 << 12) | ((qdata & 0x7) << 9) | (0x1 << 8) | (0x2 << 6) | (reg & 0x7)

def m68k_branch(cond, bdisp):
    return (0x6 << 12) | ((cond & 0xF) << 8) | (bdisp & 0xFF)

def m68k_move_to_mem(src_reg, dst_reg):
    return (0x2 << 12) | ((dst_reg & 0x7) << 9) | (0x2 << 6) | (src_reg & 0x7)

def m68k_move_from_mem(src_reg, dst_reg):
    return (0x2 << 12) | ((dst_reg & 0x7) << 9) | (0x0 << 6) | (0x2 << 3) | (src_reg & 0x7)

def m68k_halt():
    return 0x4AFC

# Instructions
def m68k_add(rd, reg): return m68k_alu(0xD, rd, reg)
def m68k_sub(rd, reg): return m68k_alu(0x9, rd, reg)
def m68k_and(rd, reg): return m68k_alu(0xC, rd, reg)
def m68k_or(rd, reg):  return m68k_alu(0x8, rd, reg)
def m68k_eor(rd, reg): return m68k_alu(0xB, rd, reg)

def m68k_bne(bdisp):   return m68k_branch(0x6, bdisp)
def m68k_beq(bdisp):   return m68k_branch(0x7, bdisp)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:04X} ")
        f.write("\n")

def write_elf_m68k(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack(">H", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for Motorola 68000 (EM_68K = 4, Big Endian)
    e_ident = b"\x7fELF\x01\x02\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 4       # EM_68K
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

    ehdr = struct.pack(">16sHHIIIIIHHHHHH",
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
    p_align = 2

    phdr = struct.pack(">IIIIIIII",
        p_type, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_flags, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_m68k_alu(outer_loops=100, inner_loops=100):
    # D6=outer, D1=inner, D0=acc, D2=7, D3=11, D4=13, D5=17
    code = [
        m68k_moveq(outer_loops & 0x7F, 6), # 0
        m68k_moveq(0, 0),                 # 1
        m68k_moveq(7, 2),                 # 2
        m68k_moveq(11, 3),                # 3
        m68k_moveq(13, 4),                # 4
        m68k_moveq(17, 5),                # 5
    ]
    outer_start = len(code)               # 6
    code.append(m68k_moveq(inner_loops & 0x7F, 1)) # 6: D1 = inner_loops
    inner_start = len(code)               # 7
    code.append(m68k_add(0, 2))           # 7
    code.append(m68k_eor(0, 3))           # 8
    code.append(m68k_add(0, 4))           # 9
    code.append(m68k_mul(0, 5))           # 10
    code.append(m68k_addq(1, 2))          # 11
    code.append(m68k_subq(1, 1))          # 12
    # bne inner_start: target is inner_start, current is 13
    bne_inner_disp = (inner_start - (len(code) + 1)) * 2 # (7 - 14) * 2 = -14
    code.append(m68k_bne(bne_inner_disp)) # 13
    code.append(m68k_subq(1, 6))          # 14
    # bne outer_start: target is outer_start, current is 15
    bne_outer_disp = (outer_start - (len(code) + 1)) * 2 # (6 - 16) * 2 = -20
    code.append(m68k_bne(bne_outer_disp)) # 15
    code.append(m68k_halt())              # 16
    return code

def main():
    print("=" * 65)
    print(" ArchC Motorola 68000 (m68k) Verification & Performance")
    print("=" * 65)

    test_files = [
        ("m68k_alu.hex", gen_m68k_alu(100, 100), write_hex),
        ("m68k_alu.elf", gen_m68k_alu(100, 100), write_elf_m68k),
    ]

    sim = "./m68k.x"
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
