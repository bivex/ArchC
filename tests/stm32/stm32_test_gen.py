#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (16-bit Thumb Little Endian)
def thumb_imm8(op, subop, rd, imm8):
    return ((op & 0xF) << 12) | ((subop & 0x1) << 11) | ((rd & 0x7) << 8) | (imm8 & 0xFF)

def thumb_alu3(op, subop, rm, rn, rd):
    return ((op & 0xF) << 12) | ((subop & 0x7) << 9) | ((rm & 0x7) << 6) | ((rn & 0x7) << 3) | (rd & 0x7)

def thumb_alu2(op, subop, rm, rdn):
    return ((op & 0xF) << 12) | ((subop & 0x3F) << 6) | ((rm & 0x7) << 3) | (rdn & 0x7)

def thumb_mem(op, subop, imm5, rn, rt):
    return ((op & 0xF) << 12) | ((subop & 0x1) << 11) | ((imm5 & 0x1F) << 6) | ((rn & 0x7) << 3) | (rt & 0x7)

def thumb_bcond(op, cond, bdisp8):
    return ((op & 0xF) << 12) | ((cond & 0xF) << 8) | (bdisp8 & 0xFF)

def thumb_bunc(bdisp11):
    return (0xE << 12) | (bdisp11 & 0x7FF)

def thumb_bkpt():
    return (0xB << 12)

# Instructions
def stm32_movs(rd, imm8):    return thumb_imm8(0x2, 0, rd, imm8)
def stm32_cmp(rd, imm8):     return thumb_imm8(0x2, 1, rd, imm8)
def stm32_adds_imm(rd, imm8):return thumb_imm8(0x3, 0, rd, imm8)
def stm32_subs_imm(rd, imm8):return thumb_imm8(0x3, 1, rd, imm8)

def stm32_adds_reg(rd, rn, rm): return thumb_alu3(0x1, 0x6, rm, rn, rd)
def stm32_subs_reg(rd, rn, rm): return thumb_alu3(0x1, 0x7, rm, rn, rd)

def stm32_ands(rdn, rm):     return thumb_alu2(0x4, 0x00, rm, rdn)
def stm32_eors(rdn, rm):     return thumb_alu2(0x4, 0x01, rm, rdn)
def stm32_orrs(rdn, rm):     return thumb_alu2(0x4, 0x0C, rm, rdn)
def stm32_muls(rdn, rm):     return thumb_alu2(0x4, 0x0D, rm, rdn)

def stm32_str(rt, rn, imm5): return thumb_mem(0x6, 0, imm5, rn, rt)
def stm32_ldr(rt, rn, imm5): return thumb_mem(0x6, 1, imm5, rn, rt)

def stm32_beq(bdisp):        return thumb_bcond(0xD, 0x0, bdisp)
def stm32_bne(bdisp):        return thumb_bcond(0xD, 0x1, bdisp)
def stm32_b(bdisp):          return thumb_bunc(bdisp)
def stm32_bkpt():            return thumb_bkpt()

def write_hex(filename, instrs, base_addr=0x08000000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:04X} ")
        f.write("\n")

def write_elf_stm32(filename, instrs, entry=0x08000000):
    code_bytes = b"".join(struct.pack("<H", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for ARM Cortex-M3 (EM_ARM = 40 = 0x28)
    e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 40      # EM_ARM
    e_version = 1
    e_entry = entry | 1 # Thumb bit set
    e_phoff = ehdr_size
    e_shoff = 0
    e_flags = 0x05000000 # EF_ARM_EABI_VER5
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
    p_align = 2

    phdr = struct.pack("<IIIIIIII",
        p_type, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_flags, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_stm32_alu(outer_loops=250, inner_loops=200):
    # R6=outer, R1=inner, R0=acc, R2=7, R3=11, R4=13, R5=17
    code = [
        stm32_movs(6, outer_loops & 0xFF), # 0
        stm32_movs(0, 0),                 # 1
        stm32_movs(2, 7),                 # 2
        stm32_movs(3, 11),                # 3
        stm32_movs(4, 13),                # 4
        stm32_movs(5, 17),                # 5
    ]
    outer_start = len(code)               # 6
    code.append(stm32_movs(1, inner_loops & 0xFF)) # 6: R1 = inner_loops
    inner_start = len(code)               # 7
    code.append(stm32_adds_reg(0, 0, 2))  # 7: R0 += R2
    code.append(stm32_eors(0, 3))         # 8: R0 ^= R3
    code.append(stm32_adds_reg(0, 0, 4))  # 9: R0 += R4
    code.append(stm32_muls(0, 5))         # 10: R0 *= R5
    code.append(stm32_adds_imm(2, 1))     # 11: R2 += 1
    code.append(stm32_subs_imm(1, 1))     # 12: R1 -= 1
    bne_inner_disp = inner_start - (len(code) + 2) # 7 - 15 = -8
    code.append(stm32_bne(bne_inner_disp)) # 13
    code.append(stm32_subs_imm(6, 1))     # 14: R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2) # 6 - 17 = -11
    code.append(stm32_bne(bne_outer_disp)) # 15
    code.append(stm32_bkpt())             # 16
    return code

def gen_stm32_mem(outer_loops=200, inner_loops=200):
    # R6=outer, R1=inner, R7=SP (0x20005000), R0=0x55, R2=0xAA
    code = [
        stm32_movs(6, outer_loops & 0xFF), # 0
        stm32_movs(0, 0x55),              # 1
        stm32_movs(2, 0xAA),              # 2
    ]
    outer_start = len(code)               # 3
    code.append(stm32_movs(1, inner_loops & 0xFF)) # 3
    inner_start = len(code)               # 4
    code.append(stm32_str(0, 7, 0))       # 4: [R7 + 0] = R0
    code.append(stm32_str(2, 7, 1))       # 5: [R7 + 4] = R2
    code.append(stm32_ldr(3, 7, 0))       # 6: R3 = [R7 + 0]
    code.append(stm32_ldr(4, 7, 1))       # 7: R4 = [R7 + 4]
    code.append(stm32_adds_reg(3, 3, 4))  # 8: R3 = R3 + R4
    code.append(stm32_adds_imm(0, 1))     # 9: R0 += 1
    code.append(stm32_subs_imm(1, 1))     # 10: R1 -= 1
    bne_inner_disp = inner_start - (len(code) + 2) # 4 - 13 = -9
    code.append(stm32_bne(bne_inner_disp)) # 11
    code.append(stm32_subs_imm(6, 1))     # 12: R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2) # 3 - 15 = -12
    code.append(stm32_bne(bne_outer_disp)) # 13
    code.append(stm32_bkpt())             # 14
    return code

def main():
    print("=" * 65)
    print(" ArchC STM32F103 (ARM Cortex-M3 / Blue Pill) Performance")
    print("=" * 65)

    test_files = [
        ("stm32_alu.elf", gen_stm32_alu(250, 200), write_elf_stm32),
        ("stm32_mem.elf", gen_stm32_mem(200, 200), write_elf_stm32),
    ]

    sim = "./stm32.x"
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
