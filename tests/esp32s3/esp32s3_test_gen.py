#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (32-bit Little Endian)
def xtensa_rrr(op, r, s, t, op1=0, op2=0):
    return ((op & 0xFF) << 24) | ((op1 & 0xF) << 20) | ((r & 0xF) << 16) | ((s & 0xF) << 12) | ((t & 0xF) << 8) | (op2 & 0xFF)

def xtensa_rri8(op, s, t, imm8):
    return ((op & 0xFF) << 24) | ((imm8 & 0xFF) << 16) | ((s & 0xF) << 12) | ((t & 0xF) << 8)

def xtensa_ri16(op, t, imm16):
    return ((op & 0xFF) << 24) | ((t & 0xF) << 20) | ((imm16 & 0xFFFF) << 4)

def xtensa_l32i(op, s, t, disp):
    return ((op & 0xFF) << 24) | ((disp & 0xFF) << 16) | ((s & 0xF) << 12) | ((t & 0xF) << 8)

def xtensa_branch(op, s, t, bdisp):
    return ((op & 0xFF) << 24) | ((bdisp & 0xFFF) << 12) | ((s & 0xF) << 8) | ((t & 0xF) << 4)

def xtensa_pie(op, qr, qs, qt, mode):
    return ((op & 0xFF) << 24) | ((qr & 0x7) << 21) | ((qs & 0x7) << 18) | ((qt & 0x7) << 15) | ((mode & 0x7F) << 8)

def xtensa_halt():
    return (0x7F << 24)

# Instructions
def s3_add(r, s, t):      return xtensa_rrr(0x00, r, s, t, 0x0, 0x08)
def s3_sub(r, s, t):      return xtensa_rrr(0x00, r, s, t, 0x0, 0x0C)
def s3_xor(r, s, t):      return xtensa_rrr(0x00, r, s, t, 0x0, 0x03)
def s3_mull(r, s, t):     return xtensa_rrr(0x00, r, s, t, 0x2, 0x08)

def s3_addi(t, s, imm8):  return xtensa_rri8(0x02, s, t, imm8)
def s3_movi(t, imm16):    return xtensa_ri16(0x0A, t, imm16)

def s3_l32i(t, s, disp):  return xtensa_l32i(0x06, s, t, disp)
def s3_s32i(t, s, disp):  return xtensa_l32i(0x07, s, t, disp)

def s3_bne(s, t, bdisp):  return xtensa_branch(0x17, s, t, bdisp)
def s3_beq(s, t, bdisp):  return xtensa_branch(0x16, s, t, bdisp)

# Vector AI Extension (PIE)
def s3_vadd_s16(qr, qs, qt): return xtensa_pie(0x30, qr, qs, qt, 0x01)
def s3_vmul_s16(qr, qs, qt): return xtensa_pie(0x30, qr, qs, qt, 0x02)
def s3_vdot_s8(ar, qs, qt):  return xtensa_pie(0x30, ar, qs, qt, 0x03)
def s3_vld_q(qr, as_reg):    return xtensa_pie(0x30, qr, as_reg, 0, 0x04)
def s3_vst_q(qr, as_reg):    return xtensa_pie(0x30, qr, as_reg, 0, 0x05)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:08X} ")
        f.write("\n")

def write_elf_esp32s3(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for Tensilica Xtensa
    e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 94      # EM_XTENSA
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

def gen_esp32s3_ai_bench(outer_loops=1000, inner_loops=5000):
    # Tests ESP32-S3 Vector AI instructions (Dot product and 16-bit packed SIMD)
    code = [
        s3_movi(6, outer_loops), # 0: A6 = outer_loops
        s3_movi(0, 0),           # 1: A0 = acc
        s3_movi(15, 0x4000),     # 2: A15 = buffer
        s3_movi(7, 0),           # 3: A7 = 0
    ]
    outer_start = len(code)      # 4
    code.append(s3_movi(1, inner_loops)) # 4: A1 = inner_loops
    inner_start = len(code)      # 5
    code.append(s3_vadd_s16(0, 1, 2))  # 5: Q0 = Q1 + Q2 (Packed 16-bit)
    code.append(s3_vmul_s16(0, 0, 3))  # 6: Q0 = Q0 * Q3
    code.append(s3_vdot_s8(0, 4, 5))   # 7: A0 += dot(Q4, Q5) (Int8 neural net dot prod)
    code.append(s3_addi(0, 0, 1))      # 8: A0 += 1
    code.append(s3_addi(1, 1, -1))     # 9: A1 -= 1
    bne_inner_disp = inner_start - len(code) # 5 - 10 = -5
    code.append(s3_bne(1, 7, bne_inner_disp)) # 10
    code.append(s3_addi(6, 6, -1))     # 11: A6 -= 1
    bne_outer_disp = outer_start - len(code) # 4 - 12 = -8
    code.append(s3_bne(6, 7, bne_outer_disp)) # 12
    code.append(xtensa_halt())         # 13
    return code

def main():
    print("=" * 65)
    print(" ArchC ESP32-S3 (Xtensa LX7 + Vector AI / PIE) Performance")
    print("=" * 65)

    test_files = [
        ("esp32s3_ai.hex", gen_esp32s3_ai_bench(1000, 5000), write_hex),
        ("esp32s3_ai.elf", gen_esp32s3_ai_bench(1000, 5000), write_elf_esp32s3),
    ]

    sim = "./esp32s3.x"
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
