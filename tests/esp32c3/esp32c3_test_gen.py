#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (32-bit Little Endian RISC-V)
def encode_r(funct7, rs2, rs1, funct3, rd, op):
    return ((funct7 & 0x7F) << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((rd & 0x1F) << 7) | (op & 0x7F)

def encode_i(imm12, rs1, funct3, rd, op):
    return ((imm12 & 0xFFF) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | ((rd & 0x1F) << 7) | (op & 0x7F)

def encode_s(imm12, rs2, rs1, funct3, op):
    imm = imm12 & 0xFFF
    imm11_5 = (imm >> 5) & 0x7F
    imm4_0 = imm & 0x1F
    return (imm11_5 << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | (imm4_0 << 7) | (op & 0x7F)

def encode_b(imm13, rs2, rs1, funct3, op):
    imm = imm13 & 0x1FFF
    b12 = (imm >> 12) & 0x1
    b10_5 = (imm >> 5) & 0x3F
    b4_1 = (imm >> 1) & 0xF
    b11 = (imm >> 11) & 0x1
    return (b12 << 31) | (b10_5 << 25) | ((rs2 & 0x1F) << 20) | ((rs1 & 0x1F) << 15) | ((funct3 & 0x7) << 12) | (b4_1 << 8) | (b11 << 7) | (op & 0x7F)

def encode_u(imm20, rd, op):
    return ((imm20 & 0xFFFFF) << 12) | ((rd & 0x1F) << 7) | (op & 0x7F)

def encode_j(imm21, rd, op):
    b20 = (imm21 >> 20) & 0x1
    b10_1 = (imm21 >> 1) & 0x3FF
    b11 = (imm21 >> 11) & 0x1
    b19_12 = (imm21 >> 12) & 0xFF
    return (b20 << 31) | (b10_1 << 21) | (b11 << 20) | (b19_12 << 12) | ((rd & 0x1F) << 7) | (op & 0x7F)

# Instructions
def rv_add(rd, rs1, rs2):  return encode_r(0x00, rs2, rs1, 0x0, rd, 0x33)
def rv_sub(rd, rs1, rs2):  return encode_r(0x20, rs2, rs1, 0x0, rd, 0x33)
def rv_xor(rd, rs1, rs2):  return encode_r(0x00, rs2, rs1, 0x4, rd, 0x33)
def rv_mul(rd, rs1, rs2):  return encode_r(0x01, rs2, rs1, 0x0, rd, 0x33)

def rv_addi(rd, rs1, imm): return encode_i(imm, rs1, 0x0, rd, 0x13)
def rv_bne(rs1, rs2, imm): return encode_b(imm, rs2, rs1, 0x1, 0x63)
def rv_beq(rs1, rs2, imm): return encode_b(imm, rs2, rs1, 0x0, 0x63)

def rv_lw(rd, rs1, imm):   return encode_i(imm, rs1, 0x2, rd, 0x03)
def rv_sw(rs2, rs1, imm):  return encode_s(imm, rs2, rs1, 0x2, 0x23)
def rv_lui(rd, imm20):     return encode_u(imm20, rd, 0x37)
def rv_halt():             return 0x00000000

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:08X} ")
        f.write("\n")

def write_elf_esp32c3(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for RISC-V (EM_RISCV = 243 = 0xF3)
    e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 243     # EM_RISCV
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

def rv_li(rd, val):
    if -2048 <= val <= 2047:
        return [rv_addi(rd, 0, val)]
    hi = (val + 0x800) >> 12
    lo = val - (hi << 12)
    return [rv_lui(rd, hi), rv_addi(rd, rd, lo)]

def gen_esp32c3_alu(outer_loops=1000, inner_loops=5000):
    # x6=outer, x1=inner, x10=acc, x2=7, x3=11, x4=13, x5=17
    code = []
    code += rv_li(6, outer_loops)   # x6 = outer_loops
    code += rv_li(10, 0)            # x10 = 0
    code += rv_li(2, 7)             # x2 = 7
    code += rv_li(3, 11)            # x3 = 11
    code += rv_li(4, 13)            # x4 = 13
    code += rv_li(5, 17)            # x5 = 17

    outer_start = len(code)
    code += rv_li(1, inner_loops)   # x1 = inner_loops
    inner_start = len(code)
    code.append(rv_add(10, 10, 2))  # x10 += x2
    code.append(rv_xor(10, 10, 3))  # x10 ^= x3
    code.append(rv_add(10, 10, 4))  # x10 += x4
    code.append(rv_mul(10, 10, 5))  # x10 *= x5 (Hardware M extension)
    code.append(rv_addi(2, 2, 1))   # x2 += 1
    code.append(rv_addi(1, 1, -1))  # x1 -= 1
    bne_inner_disp = (inner_start - len(code)) * 4
    code.append(rv_bne(1, 0, bne_inner_disp))
    code.append(rv_addi(6, 6, -1))  # x6 -= 1
    bne_outer_disp = (outer_start - len(code)) * 4
    code.append(rv_bne(6, 0, bne_outer_disp))
    code.append(rv_halt())
    return code

def main():
    print("=" * 65)
    print(" ArchC ESP32-C3 (Single-Core 32-bit RISC-V RV32IMC) Performance")
    print("=" * 65)

    test_files = [
        ("esp32c3_alu.hex", gen_esp32c3_alu(1000, 5000), write_hex),
        ("esp32c3_alu.elf", gen_esp32c3_alu(1000, 5000), write_elf_esp32c3),
    ]

    sim = "./esp32c3.x"
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
