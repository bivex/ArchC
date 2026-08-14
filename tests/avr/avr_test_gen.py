#!/usr/bin/env python3
import struct
import subprocess
import os

# Format Encoders (16-bit Little Endian)
def avr_2r(op, rd, rr):
    return ((op & 0x3F) << 10) | ((rd & 0x1F) << 5) | (rr & 0x1F)

def avr_1r(op, rd, func):
    return ((op & 0x7F) << 9) | ((rd & 0x1F) << 4) | (func & 0xF)

def avr_imm(op, rd_hi, val):
    k_hi = (val >> 4) & 0xF
    k_lo = val & 0xF
    return ((op & 0xF) << 12) | (k_hi << 8) | ((rd_hi & 0xF) << 4) | k_lo

def avr_branch(op, k_br, cond):
    return ((op & 0x3F) << 10) | ((k_br & 0x7F) << 3) | (cond & 0x7)

def avr_rjmp(k_jmp):
    return (0xC << 12) | (k_jmp & 0xFFF)

def avr_ptr(op, rd, ptr):
    return ((op & 0x3F) << 10) | ((rd & 0x1F) << 5) | (ptr & 0x1F)

def avr_halt():
    return 0x9588

# Instructions
def avr_add(rd, rr):     return avr_2r(0x03, rd, rr)
def avr_sub(rd, rr):     return avr_2r(0x06, rd, rr)
def avr_and(rd, rr):     return avr_2r(0x08, rd, rr)
def avr_or(rd, rr):      return avr_2r(0x0A, rd, rr)
def avr_eor(rd, rr):     return avr_2r(0x09, rd, rr)
def avr_mov(rd, rr):     return avr_2r(0x0B, rd, rr)

def avr_inc(rd):         return avr_1r(0x4B, rd, 0x3)
def avr_dec(rd):         return avr_1r(0x4B, rd, 0xA)
def avr_clr(rd):         return avr_1r(0x4B, rd, 0x0)

def avr_ldi(rd_num, val): return avr_imm(0xE, rd_num - 16, val)
def avr_subi(rd_num, val): return avr_imm(0x5, rd_num - 16, val)

def avr_brne(k_br):      return avr_branch(0x3D, k_br, 0x1)
def avr_breq(k_br):      return avr_branch(0x3D, k_br, 0x0)

def avr_ld_x(rd):        return avr_ptr(0x24, rd, 0x0C)
def avr_st_x(rd):        return avr_ptr(0x25, rd, 0x0C)

def write_hex(filename, instrs, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:04X} ")
        f.write("\n")

def write_elf_avr(filename, instrs, entry=0x1000):
    code_bytes = b"".join(struct.pack("<H", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for AVR (EM_AVR = 83 = 0x53)
    e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 83      # EM_AVR
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
    p_align = 2

    phdr = struct.pack("<IIIIIIII",
        p_type, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_flags, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_avr_alu(outer_loops=200, inner_loops=250):
    # R24=outer, R25=inner, R16=acc, R17=7, R18=11, R19=13
    code = [
        avr_ldi(24, outer_loops), # 0: R24 = outer_loops
        avr_clr(16),              # 1: R16 = 0
        avr_ldi(17, 7),           # 2: R17 = 7
        avr_ldi(18, 11),          # 3: R18 = 11
        avr_ldi(19, 13),          # 4: R19 = 13
    ]
    outer_start = len(code)       # 5: reload inner
    code.append(avr_ldi(25, inner_loops)) # 5: R25 = inner_loops
    inner_start = len(code)       # 6
    code += [
        avr_add(16, 17),          # 6: R16 += R17
        avr_eor(16, 18),          # 7: R16 ^= R18
        avr_add(16, 19),          # 8: R16 += R19
        avr_inc(17),              # 9: R17 += 1
        avr_dec(25),              # 10: R25 -= 1 (sets Z flag)
        avr_brne(inner_start - (len(code) + 5)), # 11: 6 - 11 = -5
        avr_dec(24),              # 12: R24 -= 1
        avr_brne(outer_start - (len(code) + 7)), # 13: 5 - 13 = -8
        avr_halt()                # 14
    ]
    return code

def gen_avr_mem(outer_loops=200, inner_loops=200):
    # X pointer = R27:R26 = 0x0200
    code = [
        avr_ldi(24, outer_loops), # 0
        avr_ldi(26, 0x00),        # 1: XL = 0x00
        avr_ldi(27, 0x02),        # 2: XH = 0x02 (Address 0x0200)
        avr_ldi(16, 0x55),        # 3: R16 = 0x55
        avr_ldi(17, 0xAA),        # 4: R17 = 0xAA
    ]
    outer_start = len(code)       # 5
    code.append(avr_ldi(25, inner_loops)) # 5
    inner_start = len(code)       # 6
    code += [
        avr_st_x(16),             # 6: [X] = R16
        avr_ld_x(18),             # 7: R18 = [X]
        avr_add(18, 17),          # 8: R18 += R17
        avr_inc(16),              # 9: R16 += 1
        avr_dec(25),              # 10: R25 -= 1
        avr_brne(inner_start - (len(code) + 5)), # 11: 6 - 11 = -5
        avr_dec(24),              # 12: R24 -= 1
        avr_brne(outer_start - (len(code) + 7)), # 13: 5 - 13 = -8
        avr_halt()                # 14
    ]
    return code

def main():
    print("=" * 65)
    print(" ArchC Atmel AVR (ATmega328P / Arduino) Verification Suite")
    print("=" * 65)

    test_files = [
        ("avr_alu.hex", gen_avr_alu(200, 250), write_hex),
        ("avr_mem.hex", gen_avr_mem(200, 200), write_hex),
        ("avr_alu.elf", gen_avr_alu(200, 250), write_elf_avr),
        ("avr_mem.elf", gen_avr_mem(200, 200), write_elf_avr),
    ]

    sim = "./avr.x"
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
