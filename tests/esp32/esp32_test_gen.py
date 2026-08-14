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

def xtensa_loop(s, ldisp):
    return (0x12 << 24) | ((ldisp & 0xFF) << 16) | ((s & 0xF) << 12)

def xtensa_halt():
    return (0x7F << 24)

# Instructions
def esp32_add(r, s, t):   return xtensa_rrr(0x00, r, s, t, 0x0, 0x08)
def esp32_sub(r, s, t):   return xtensa_rrr(0x00, r, s, t, 0x0, 0x0C)
def esp32_and(r, s, t):   return xtensa_rrr(0x00, r, s, t, 0x0, 0x01)
def esp32_or(r, s, t):    return xtensa_rrr(0x00, r, s, t, 0x0, 0x02)
def esp32_xor(r, s, t):   return xtensa_rrr(0x00, r, s, t, 0x0, 0x03)
def esp32_mull(r, s, t):  return xtensa_rrr(0x00, r, s, t, 0x2, 0x08)

def esp32_addi(t, s, imm8): return xtensa_rri8(0x02, s, t, imm8)
def esp32_movi(t, imm16):   return xtensa_ri16(0x0A, t, imm16)

def esp32_l32i(t, s, disp): return xtensa_l32i(0x06, s, t, disp)
def esp32_s32i(t, s, disp): return xtensa_l32i(0x07, s, t, disp)

def esp32_bne(s, t, bdisp): return xtensa_branch(0x17, s, t, bdisp)
def esp32_beq(s, t, bdisp): return xtensa_branch(0x16, s, t, bdisp)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:08X} ")
        f.write("\n")

def write_elf_esp32(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<I", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for Tensilica Xtensa (EM_XTENSA = 94 = 0x5E)
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

def gen_esp32_alu(outer_loops=1000, inner_loops=5000):
    # A6=outer, A1=inner, A0=acc, A2=7, A3=11, A4=13, A5=17, A7=0 (zero)
    code = [
        esp32_movi(6, outer_loops), # 0: A6 = outer_loops
        esp32_movi(0, 0),           # 1: A0 = 0
        esp32_movi(2, 7),           # 2: A2 = 7
        esp32_movi(3, 11),          # 3: A3 = 11
        esp32_movi(4, 13),          # 4: A4 = 13
        esp32_movi(5, 17),          # 5: A5 = 17
        esp32_movi(7, 0),           # 6: A7 = 0
    ]
    outer_start = len(code)         # 7
    code.append(esp32_movi(1, inner_loops)) # 7: A1 = inner_loops
    inner_start = len(code)         # 8
    code.append(esp32_add(0, 0, 2)) # 8: A0 += A2
    code.append(esp32_xor(0, 0, 3)) # 9: A0 ^= A3
    code.append(esp32_add(0, 0, 4)) # 10: A0 += A4
    code.append(esp32_mull(0, 0, 5))# 11: A0 *= A5
    code.append(esp32_addi(2, 2, 1))# 12: A2 += 1
    code.append(esp32_addi(1, 1, -1))# 13: A1 -= 1
    # bne A1, A7, inner_start
    bne_inner_disp = inner_start - len(code) # 8 - 14 = -6
    code.append(esp32_bne(1, 7, bne_inner_disp)) # 14
    code.append(esp32_addi(6, 6, -1))# 15: A6 -= 1
    # bne A6, A7, outer_start
    bne_outer_disp = outer_start - len(code) # 7 - 16 = -9
    code.append(esp32_bne(6, 7, bne_outer_disp)) # 16
    code.append(xtensa_halt())      # 17
    return code

def gen_esp32_mem(outer_loops=1000, inner_loops=3000):
    # A6=outer, A1=inner, A15 ($sp)=0x4000, A0=0x1234, A2=0x5678, A7=0
    code = [
        esp32_movi(6, outer_loops), # 0
        esp32_movi(15, 0x4000),     # 1: A15 = 0x4000
        esp32_movi(0, 0x1234),      # 2: A0 = 0x1234
        esp32_movi(2, 0x5678),      # 3: A2 = 0x5678
        esp32_movi(7, 0),           # 4: A7 = 0
    ]
    outer_start = len(code)         # 5
    code.append(esp32_movi(1, inner_loops)) # 5
    inner_start = len(code)         # 6
    code.append(esp32_s32i(0, 15, 0)) # 6: [A15 + 0] = A0
    code.append(esp32_s32i(2, 15, 1)) # 7: [A15 + 4] = A2
    code.append(esp32_l32i(3, 15, 0)) # 8: A3 = [A15 + 0]
    code.append(esp32_l32i(4, 15, 1)) # 9: A4 = [A15 + 4]
    code.append(esp32_add(3, 3, 4))   # 10: A3 = A3 + A4
    code.append(esp32_addi(0, 0, 1))  # 11: A0 += 1
    code.append(esp32_addi(1, 1, -1)) # 12: A1 -= 1
    bne_inner_disp = inner_start - len(code) # 6 - 13 = -7
    code.append(esp32_bne(1, 7, bne_inner_disp)) # 13
    code.append(esp32_addi(6, 6, -1)) # 14: A6 -= 1
    bne_outer_disp = outer_start - len(code) # 5 - 15 = -10
    code.append(esp32_bne(6, 7, bne_outer_disp)) # 15
    code.append(xtensa_halt())        # 16
    return code

def main():
    print("=" * 65)
    print(" ArchC ESP32 DevKit V1 (Tensilica Xtensa LX6) Performance")
    print("=" * 65)

    test_files = [
        ("esp32_alu.hex", gen_esp32_alu(1000, 5000), write_hex),
        ("esp32_mem.hex", gen_esp32_mem(1000, 3000), write_hex),
        ("esp32_alu.elf", gen_esp32_alu(1000, 5000), write_elf_esp32),
        ("esp32_mem.elf", gen_esp32_mem(1000, 3000), write_elf_esp32),
    ]

    sim = "./esp32.x"
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
