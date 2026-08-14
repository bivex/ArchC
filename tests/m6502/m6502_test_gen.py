#!/usr/bin/env python3
import struct
import subprocess
import os

# Helper byte encoders
def m6502_imp(op):          return [op]
def m6502_imm(op, imm):     return [op, imm & 0xFF]
def m6502_zp(op, zp):       return [op, zp & 0xFF]
def m6502_abs(op, addr):    return [op, addr & 0xFF, (addr >> 8) & 0xFF]
def m6502_branch(op, disp): return [op, disp & 0xFF]

# Instructions
def m6502_tax():         return m6502_imp(0xAA)
def m6502_txa():         return m6502_imp(0x8A)
def m6502_inx():         return m6502_imp(0xE8)
def m6502_dex():         return m6502_imp(0xCA)
def m6502_iny():         return m6502_imp(0xC8)
def m6502_dey():         return m6502_imp(0x88)
def m6502_clc():         return m6502_imp(0x18)
def m6502_brk():         return m6502_imp(0x00)

def m6502_lda_imm(imm):  return m6502_imm(0xA9, imm)
def m6502_ldx_imm(imm):  return m6502_imm(0xA2, imm)
def m6502_ldy_imm(imm):  return m6502_imm(0xA0, imm)
def m6502_adc_imm(imm):  return m6502_imm(0x69, imm)
def m6502_and_imm(imm):  return m6502_imm(0x29, imm)
def m6502_ora_imm(imm):  return m6502_imm(0x09, imm)
def m6502_eor_imm(imm):  return m6502_imm(0x49, imm)

def m6502_lda_zp(zp):    return m6502_zp(0xA5, zp)
def m6502_sta_zp(zp):    return m6502_zp(0x85, zp)
def m6502_ldx_zp(zp):    return m6502_zp(0xA6, zp)
def m6502_stx_zp(zp):    return m6502_zp(0x86, zp)

def m6502_bne(disp):     return m6502_branch(0xD0, disp)
def m6502_beq(disp):     return m6502_branch(0xF0, disp)

def write_hex(filename, byte_list, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for b in byte_list:
            f.write(f"0x{b:02X} ")
        f.write("\n")

def write_elf_m6502(filename, byte_list, entry=0x1000):
    code_bytes = bytes(byte_list)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for MOS 6502
    e_ident = b"\x7fELF\x01\x01\x01\x00" + b"\x00" * 8
    e_type = 2          # ET_EXEC
    e_machine = 0x6502  # Custom EM_6502
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
    p_align = 1

    phdr = struct.pack("<IIIIIIII",
        p_type, p_offset, p_vaddr, p_paddr,
        p_filesz, p_memsz, p_flags, p_align
    )

    with open(filename, "wb") as f:
        f.write(ehdr)
        f.write(phdr)
        f.write(code_bytes)

def gen_m6502_alu(outer_loops=200, inner_loops=250):
    # Y = outer_loops, X = inner_loops, A = acc
    bytes_out = []
    bytes_out += m6502_ldy_imm(outer_loops) # 2 bytes
    bytes_out += m6502_lda_imm(0)           # 2 bytes
    bytes_out += m6502_clc()                # 1 byte

    outer_start_offset = len(bytes_out)
    bytes_out += m6502_ldx_imm(inner_loops) # 2 bytes
    inner_start_offset = len(bytes_out)

    bytes_out += m6502_adc_imm(7)           # 2 bytes
    bytes_out += m6502_eor_imm(11)          # 2 bytes
    bytes_out += m6502_adc_imm(13)          # 2 bytes
    bytes_out += m6502_dex()                # 1 byte (sets Z flag when X == 0)

    # bne inner_start: branch is 2 bytes long, target is inner_start_offset
    # displacement is relative to PC after bne instruction (len(bytes_out) + 2)
    bne_inner_disp = inner_start_offset - (len(bytes_out) + 2)
    bytes_out += m6502_bne(bne_inner_disp)  # 2 bytes

    bytes_out += m6502_dey()                # 1 byte (sets Z flag when Y == 0)
    bne_outer_disp = outer_start_offset - (len(bytes_out) + 2)
    bytes_out += m6502_bne(bne_outer_disp)  # 2 bytes
    bytes_out += m6502_brk()                # 1 byte
    return bytes_out

def gen_m6502_mem(outer_loops=200, inner_loops=200):
    # Zero-page memory addresses: $20, $21
    bytes_out = []
    bytes_out += m6502_ldy_imm(outer_loops) # 2 bytes
    bytes_out += m6502_lda_imm(0x55)        # 2 bytes
    bytes_out += m6502_sta_zp(0x20)         # 2 bytes

    outer_start_offset = len(bytes_out)
    bytes_out += m6502_ldx_imm(inner_loops) # 2 bytes
    inner_start_offset = len(bytes_out)

    bytes_out += m6502_lda_zp(0x20)         # 2 bytes
    bytes_out += m6502_adc_imm(1)           # 2 bytes
    bytes_out += m6502_sta_zp(0x20)         # 2 bytes
    bytes_out += m6502_dex()                # 1 byte

    bne_inner_disp = inner_start_offset - (len(bytes_out) + 2)
    bytes_out += m6502_bne(bne_inner_disp)  # 2 bytes

    bytes_out += m6502_dey()                # 1 byte
    bne_outer_disp = outer_start_offset - (len(bytes_out) + 2)
    bytes_out += m6502_bne(bne_outer_disp)  # 2 bytes
    bytes_out += m6502_brk()                # 1 byte
    return bytes_out

def main():
    print("=" * 65)
    print(" ArchC MOS Technology 6502 (NES / Apple II) Verification Suite")
    print("=" * 65)

    test_files = [
        ("m6502_alu.hex", gen_m6502_alu(200, 250), write_hex),
        ("m6502_mem.hex", gen_m6502_mem(200, 200), write_hex),
        ("m6502_alu.elf", gen_m6502_alu(200, 250), write_elf_m6502),
        ("m6502_mem.elf", gen_m6502_mem(200, 200), write_elf_m6502),
    ]

    sim = "./m6502.x"
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
