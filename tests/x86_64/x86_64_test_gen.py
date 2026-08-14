#!/usr/bin/env python3
import struct
import subprocess
import os

# Format builders (MSB to LSB in 32-bit little-endian word)
def x64_rr(op, rm, reg, rex=0):
    return ((op & 0xFF) << 24) | ((rex & 0xF) << 20) | ((reg & 0xF) << 16) | ((rm & 0xF) << 12)

def x64_ri(op, reg, imm):
    return ((op & 0xFF) << 24) | ((reg & 0xF) << 20) | (imm & 0xFFFF)

def x64_rm(op, reg, rm, disp=0):
    return ((op & 0xFF) << 24) | ((reg & 0xF) << 20) | ((rm & 0xF) << 16) | (disp & 0xFFFF)

def x64_j(op, offset):
    return ((op & 0xFF) << 24) | (offset & 0xFFFFFF)

def x64_hlt():
    return 0xF4 << 24

# Instructions
def x64_movq_ri(reg, imm): return x64_ri(0xB8, reg, imm)
def x64_addq_ri(reg, imm): return x64_ri(0x81, reg, imm)
def x64_subq_ri(reg, imm): return x64_ri(0x83, reg, imm)
def x64_addq_rr(rm, reg):  return x64_rr(0x01, rm, reg)
def x64_subq_rr(rm, reg):  return x64_rr(0x29, rm, reg)
def x64_xorq_rr(rm, reg):  return x64_rr(0x31, rm, reg)
def x64_movq_rr(rm, reg):  return x64_rr(0x89, rm, reg)
def x64_imulq_rr(rm, reg): return x64_rr(0xAF, rm, reg)
def x64_movq_mr(disp, rm, reg): return x64_rm(0x88, reg, rm, disp)
def x64_movq_rm(reg, disp, rm): return x64_rm(0x8B, reg, rm, disp)
def x64_jne(offset):       return x64_j(0x75, offset)
def x64_jmp(offset):       return x64_j(0xE9, offset)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:016X} ")
        for i in range(0, len(instrs), 2):
            w0 = instrs[i]
            w1 = instrs[i+1] if i+1 < len(instrs) else (0xF4 << 24)
            w64 = (w0 & 0xFFFFFFFF) | ((w1 & 0xFFFFFFFF) << 32)
            f.write(f"0x{w64:016X} ")
        f.write("\n")

def write_elf64_little(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 64
    phdr_size = 56
    file_size = len(code_bytes)

    # ELF64 Header for x86-64 (EM_X86_64 = 62)
    e_ident = b"\x7fELF\x02\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 62      # EM_X86_64
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

def gen_x64_alu(outer_loops=1000, inner_loops=5000):
    # 64-bit registers: RAX=0, RCX=1, RDX=2, RBX=3, R8=8, R9=9, R10=10 (outer counter)
    code = [
        x64_movq_ri(10, outer_loops), # 0: r10 = outer_loops
        x64_movq_ri(0, 0),            # 1: rax = 0
        x64_movq_ri(2, 7),            # 2: rdx = 7
        x64_movq_ri(3, 11),           # 3: rbx = 11
        x64_movq_ri(8, 13),           # 4: r8 = 13
        x64_movq_ri(9, 17),           # 5: r9 = 17
    ]
    outer_start = len(code)
    code.append(x64_movq_ri(1, inner_loops))
    inner_start = len(code)
    code.append(x64_addq_rr(0, 2))
    code.append(x64_xorq_rr(0, 3))
    code.append(x64_addq_rr(0, 8))
    code.append(x64_imulq_rr(0, 9))
    code.append(x64_addq_ri(2, 1))
    code.append(x64_subq_ri(1, 1))
    code.append(x64_jne(inner_start - len(code)))
    code.append(x64_subq_ri(10, 1))
    code.append(x64_jne(outer_start - len(code)))
    code.append(x64_hlt())
    return code

def gen_x64_mem(outer_loops=1000, inner_loops=3000):
    code = [
        x64_movq_ri(10, outer_loops), # 0: r10 = outer_loops
        x64_movq_ri(4, 0x4000),       # 1: rsp = 0x4000 (positive 16-bit)
        x64_movq_ri(0, 0x1234),       # 2: rax = 0x1234
        x64_movq_ri(2, 0x5678),       # 3: rdx = 0x5678
    ]
    outer_start = len(code)
    code.append(x64_movq_ri(1, inner_loops))
    inner_start = len(code)
    code.append(x64_movq_mr(0, 4, 0))
    code.append(x64_movq_mr(8, 4, 2))
    code.append(x64_movq_rm(8, 0, 4))
    code.append(x64_movq_rm(9, 8, 4))
    code.append(x64_addq_rr(8, 9))
    code.append(x64_addq_ri(0, 1))
    code.append(x64_subq_ri(1, 1))
    code.append(x64_jne(inner_start - len(code)))
    code.append(x64_subq_ri(10, 1))
    code.append(x64_jne(outer_start - len(code)))
    code.append(x64_hlt())
    return code

def main():
    print("=" * 65)
    print(" ArchC Intel x86-64 (AMD64 / x64) Verification & Performance")
    print("=" * 65)

    test_files = [
        ("x64_alu.hex", gen_x64_alu(1000, 5000), write_hex),
        ("x64_mem.hex", gen_x64_mem(1000, 3000), write_hex),
        ("x64_alu.elf", gen_x64_alu(1000, 5000), write_elf64_little),
        ("x64_mem.elf", gen_x64_mem(1000, 3000), write_elf64_little),
    ]

    sim = "./x86_64.x"
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
