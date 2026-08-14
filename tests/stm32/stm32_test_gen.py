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
def stm32_bcs(bdisp):        return thumb_bcond(0xD, 0x2, bdisp)
def stm32_bcc(bdisp):        return thumb_bcond(0xD, 0x3, bdisp)
def stm32_bmi(bdisp):        return thumb_bcond(0xD, 0x4, bdisp)
def stm32_bpl(bdisp):        return thumb_bcond(0xD, 0x5, bdisp)
def stm32_bge(bdisp):        return thumb_bcond(0xD, 0xA, bdisp)
def stm32_blt(bdisp):        return thumb_bcond(0xD, 0xB, bdisp)

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

# 1. High-Performance ALU Benchmark (35M instructions)
def gen_stm32_alu():
    # 3-level nested loop: 125 * 200 * 200
    code = [
        stm32_movs(6, 125),               # R6 = outer
        stm32_movs(0, 0),                 # R0 = acc
        stm32_movs(2, 7),                 # R2 = 7
        stm32_movs(3, 11),                # R3 = 11
        stm32_movs(4, 13),                # R4 = 13
        stm32_movs(5, 17),                # R5 = 17
    ]
    outer_start = len(code)
    code.append(stm32_movs(7, 200))       # R7 = mid
    mid_start = len(code)
    code.append(stm32_movs(1, 200))       # R1 = inner
    inner_start = len(code)
    code.append(stm32_adds_reg(0, 0, 2))  # R0 += R2
    code.append(stm32_eors(0, 3))         # R0 ^= R3
    code.append(stm32_adds_reg(0, 0, 4))  # R0 += R4
    code.append(stm32_muls(0, 5))         # R0 *= R5
    code.append(stm32_adds_imm(2, 1))     # R2 += 1
    code.append(stm32_subs_imm(1, 1))     # R1 -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(stm32_bne(bne_inner_disp))# loop inner
    code.append(stm32_subs_imm(7, 1))     # R7 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(stm32_bne(bne_mid_disp))  # loop mid
    code.append(stm32_subs_imm(6, 1))     # R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(stm32_bne(bne_outer_disp))# loop outer
    code.append(stm32_bkpt())
    return code

# 2. High-Performance SRAM Memory Benchmark (25M instructions)
def gen_stm32_mem():
    # R7 = SRAM Pointer (0x20005000), 100 * 150 * 200
    code = [
        stm32_movs(6, 100),               # R6 = outer
        stm32_movs(0, 0x55),
        stm32_movs(2, 0xAA),
    ]
    outer_start = len(code)
    code.append(stm32_movs(5, 150))       # R5 = mid
    mid_start = len(code)
    code.append(stm32_movs(1, 200))       # R1 = inner
    inner_start = len(code)
    code.append(stm32_str(0, 7, 0))       # [R7 + 0] = R0
    code.append(stm32_str(2, 7, 1))       # [R7 + 4] = R2
    code.append(stm32_ldr(3, 7, 0))       # R3 = [R7 + 0]
    code.append(stm32_ldr(4, 7, 1))       # R4 = [R7 + 4]
    code.append(stm32_adds_reg(3, 3, 4))  # R3 = R3 + R4
    code.append(stm32_adds_imm(0, 1))     # R0 += 1
    code.append(stm32_subs_imm(1, 1))     # R1 -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(stm32_bne(bne_inner_disp))
    code.append(stm32_subs_imm(5, 1))     # R5 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(stm32_bne(bne_mid_disp))
    code.append(stm32_subs_imm(6, 1))     # R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(stm32_bne(bne_outer_disp))
    code.append(stm32_bkpt())
    return code

# 3. DSP / FIR Filter Simulation (Multiply-Accumulate loop)
def gen_stm32_dsp_fir():
    # 200 * 200 * 8 = ~2.5M instructions
    code = [
        stm32_movs(6, 200),            # R6 = outer
        stm32_movs(0, 0),              # Output accumulator
        stm32_movs(2, 3),              # Filter coefficient h0
        stm32_movs(3, 5),              # Filter coefficient h1
        stm32_movs(4, 7),              # Signal sample x0
        stm32_movs(5, 9),              # Signal sample x1
    ]
    outer_start = len(code)
    code.append(stm32_movs(7, 200))    # R7 = mid
    mid_start = len(code)
    code.append(stm32_movs(1, 8))      # R1 = taps
    inner_start = len(code)
    code.append(stm32_muls(4, 2))      # x0 * h0
    code.append(stm32_adds_reg(0, 0, 4)) # acc += x0*h0
    code.append(stm32_muls(5, 3))      # x1 * h1
    code.append(stm32_adds_reg(0, 0, 5)) # acc += x1*h1
    code.append(stm32_adds_imm(4, 1))  # x0 += 1
    code.append(stm32_subs_imm(1, 1))  # taps -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(stm32_bne(bne_inner_disp))
    code.append(stm32_subs_imm(7, 1))  # R7 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(stm32_bne(bne_mid_disp))
    code.append(stm32_subs_imm(6, 1))  # R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(stm32_bne(bne_outer_disp))
    code.append(stm32_bkpt())
    return code

# 4. CRC-32 / Checksum Protocol Calculation (Industrial LoRa / CAN Packet)
def gen_stm32_crc():
    # 200 * 200 * 64 = ~20M instructions
    code = [
        stm32_movs(6, 200),            # R6 = outer
        stm32_movs(0, 0xFF),           # Initial CRC
        stm32_movs(2, 0x82),           # Polynomial term
    ]
    outer_start = len(code)
    code.append(stm32_movs(7, 200))    # R7 = mid
    mid_start = len(code)
    code.append(stm32_movs(1, 64))     # Byte counter
    inner_start = len(code)
    code.append(stm32_eors(0, 1))      # CRC ^= byte
    code.append(stm32_muls(0, 2))      # CRC *= poly
    code.append(stm32_adds_imm(0, 3))  # CRC += 3
    code.append(stm32_subs_imm(1, 1))  # bytes -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(stm32_bne(bne_inner_disp))
    code.append(stm32_subs_imm(7, 1))  # R7 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(stm32_bne(bne_mid_disp))
    code.append(stm32_subs_imm(6, 1))  # packets -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(stm32_bne(bne_outer_disp))
    code.append(stm32_bkpt())
    return code

# 5. Full Conditional Branch & Flags Verification
def gen_stm32_cond_branch():
    code = [
        # Test 1: BEQ (equal)
        stm32_movs(0, 10),
        stm32_cmp(0, 10),              # Z=1
        stm32_beq(1),                  # skip next instruction
        stm32_bkpt(),                  # FAIL if not skipped

        # Test 2: BNE (not equal)
        stm32_movs(1, 20),
        stm32_cmp(1, 10),              # Z=0
        stm32_bne(1),                  # skip next instruction
        stm32_bkpt(),                  # FAIL if not skipped

        # Test 3: BCS (carry set)
        stm32_movs(2, 255),
        stm32_adds_imm(2, 5),          # C=1
        stm32_bcs(1),                  # skip
        stm32_bkpt(),                  # FAIL

        # Test 4: BPL (positive)
        stm32_movs(3, 50),
        stm32_cmp(3, 10),              # N=0
        stm32_bpl(1),                  # skip
        stm32_bkpt(),                  # FAIL

        # Test 5: Unconditional Branch
        stm32_b(1),
        stm32_bkpt(),                  # FAIL

        # Test 6: Final Success Pass
        stm32_movs(0, 0x42),           # Success code in R0
        stm32_bkpt(),
    ]
    return code

def main():
    print("=" * 70)
    print(" ArchC STM32F103 (ARM Cortex-M3 / Blue Pill) Comprehensive Suite")
    print("=" * 70)

    test_files = [
        ("stm32_alu.elf",         gen_stm32_alu(),            write_elf_stm32),
        ("stm32_mem.elf",         gen_stm32_mem(),            write_elf_stm32),
        ("stm32_dsp_fir.elf",     gen_stm32_dsp_fir(),        write_elf_stm32),
        ("stm32_crc.elf",         gen_stm32_crc(),            write_elf_stm32),
        ("stm32_cond_branch.elf", gen_stm32_cond_branch(),    write_elf_stm32),
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
        print(f"  {tf:<22} | Number of instructions executed: {inst_count:<12} | Simulation speed: {speed}")

if __name__ == "__main__":
    main()
