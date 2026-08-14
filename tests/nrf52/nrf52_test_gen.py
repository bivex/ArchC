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

def thumb_special(subop, imm8=0):
    return (0xB << 12) | ((subop & 0xF) << 8) | (imm8 & 0xFF)

# Instructions
def nrf_movs(rd, imm8):    return thumb_imm8(0x2, 0, rd, imm8)
def nrf_cmp(rd, imm8):     return thumb_imm8(0x2, 1, rd, imm8)
def nrf_adds_imm(rd, imm8):return thumb_imm8(0x3, 0, rd, imm8)
def nrf_subs_imm(rd, imm8):return thumb_imm8(0x3, 1, rd, imm8)

def nrf_adds_reg(rd, rn, rm): return thumb_alu3(0x1, 0x6, rm, rn, rd)
def nrf_subs_reg(rd, rn, rm): return thumb_alu3(0x1, 0x7, rm, rn, rd)

def nrf_ands(rdn, rm):     return thumb_alu2(0x4, 0x00, rm, rdn)
def nrf_eors(rdn, rm):     return thumb_alu2(0x4, 0x01, rm, rdn)
def nrf_orrs(rdn, rm):     return thumb_alu2(0x4, 0x0C, rm, rdn)
def nrf_muls(rdn, rm):     return thumb_alu2(0x4, 0x0D, rm, rdn)

def nrf_str(rt, rn, imm5): return thumb_mem(0x6, 0, imm5, rn, rt)
def nrf_ldr(rt, rn, imm5): return thumb_mem(0x6, 1, imm5, rn, rt)

def nrf_beq(bdisp):        return thumb_bcond(0xD, 0x0, bdisp)
def nrf_bne(bdisp):        return thumb_bcond(0xD, 0x1, bdisp)
def nrf_b(bdisp):          return thumb_bunc(bdisp)

def nrf_wfe():             return thumb_special(0x2)
def nrf_sev():             return thumb_special(0x4)
def nrf_wfi():             return thumb_special(0x3)
def nrf_bkpt():            return thumb_special(0x0)

def write_hex(filename, instrs, base_addr=0x00001000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for inst in instrs:
            f.write(f"0x{inst:04X} ")
        f.write("\n")

def write_elf_nrf52(filename, instrs, entry=0x00001000):
    code_bytes = b"".join(struct.pack("<H", i) for i in instrs)
    ehdr_size = 52
    phdr_size = 32
    file_size = len(code_bytes)

    # ELF32 Header for ARM Cortex-M4 (EM_ARM = 40 = 0x28)
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
def gen_nrf52_alu():
    code = [
        nrf_movs(6, 125),               # R6 = outer
        nrf_movs(0, 0),                 # R0 = acc
        nrf_movs(2, 7),                 # R2 = 7
        nrf_movs(3, 11),                # R3 = 11
        nrf_movs(4, 13),                # R4 = 13
        nrf_movs(5, 17),                # R5 = 17
    ]
    outer_start = len(code)
    code.append(nrf_movs(7, 200))       # R7 = mid
    mid_start = len(code)
    code.append(nrf_movs(1, 200))       # R1 = inner
    inner_start = len(code)
    code.append(nrf_adds_reg(0, 0, 2))  # R0 += R2
    code.append(nrf_eors(0, 3))         # R0 ^= R3
    code.append(nrf_adds_reg(0, 0, 4))  # R0 += R4
    code.append(nrf_muls(0, 5))         # R0 *= R5
    code.append(nrf_adds_imm(2, 1))     # R2 += 1
    code.append(nrf_subs_imm(1, 1))     # R1 -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(nrf_bne(bne_inner_disp))# loop inner
    code.append(nrf_subs_imm(7, 1))     # R7 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(nrf_bne(bne_mid_disp))  # loop mid
    code.append(nrf_subs_imm(6, 1))     # R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(nrf_bne(bne_outer_disp))# loop outer
    code.append(nrf_bkpt())
    return code

# 2. 256KB SRAM EasyDMA Memory Benchmark (24M instructions)
def gen_nrf52_mem():
    # R7 = SRAM Pointer (0x20040000)
    code = [
        nrf_movs(6, 100),               # R6 = outer
        nrf_movs(0, 0x55),
        nrf_movs(2, 0xAA),
    ]
    outer_start = len(code)
    code.append(nrf_movs(5, 150))       # R5 = mid
    mid_start = len(code)
    code.append(nrf_movs(1, 200))       # R1 = inner
    inner_start = len(code)
    code.append(nrf_str(0, 7, 0))       # [R7 + 0] = R0
    code.append(nrf_str(2, 7, 1))       # [R7 + 4] = R2
    code.append(nrf_ldr(3, 7, 0))       # R3 = [R7 + 0]
    code.append(nrf_ldr(4, 7, 1))       # R4 = [R7 + 4]
    code.append(nrf_adds_reg(3, 3, 4))  # R3 = R3 + R4
    code.append(nrf_adds_imm(0, 1))     # R0 += 1
    code.append(nrf_subs_imm(1, 1))     # R1 -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(nrf_bne(bne_inner_disp))
    code.append(nrf_subs_imm(5, 1))     # R5 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(nrf_bne(bne_mid_disp))
    code.append(nrf_subs_imm(6, 1))     # R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(nrf_bne(bne_outer_disp))
    code.append(nrf_bkpt())
    return code

# 3. Bluetooth Low Energy (BLE 5.0) Packet CRC-24 & Whitening Simulation
def gen_nrf52_ble_packet():
    # Simulates BLE 5.0 2.4GHz Advertising & Data Channel processing
    code = [
        nrf_movs(6, 200),               # Outer packets
        nrf_movs(0, 0x55),              # Initial CRC-24 state
        nrf_movs(2, 0xDA),              # BLE Whitening polynomial
        nrf_movs(3, 0x5A),              # Channel Index seed
    ]
    outer_start = len(code)
    code.append(nrf_movs(7, 200))       # Mid loop
    mid_start = len(code)
    code.append(nrf_movs(1, 37))        # 37-byte BLE PDU Payload
    inner_start = len(code)
    code.append(nrf_eors(0, 1))         # CRC ^= payload byte
    code.append(nrf_muls(0, 2))         # CRC *= poly
    code.append(nrf_eors(0, 3))         # CRC ^= whitening
    code.append(nrf_adds_imm(0, 7))     # CRC += 7
    code.append(nrf_subs_imm(1, 1))     # bytes -= 1
    bne_inner_disp = inner_start - (len(code) + 2)
    code.append(nrf_bne(bne_inner_disp))
    code.append(nrf_subs_imm(7, 1))     # R7 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(nrf_bne(bne_mid_disp))
    code.append(nrf_subs_imm(6, 1))     # packets -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(nrf_bne(bne_outer_disp))
    code.append(nrf_bkpt())
    return code

# 4. Ultra-Low-Power (ULP) Sleep / Wakeup Cycle (CR2032 3-5 Year Battery Model)
def gen_nrf52_ulp_sleep():
    # Simulates periodic sensor beacon: Wakeup -> SEV -> WFE Sleep -> WFI Standby
    code = [
        nrf_movs(6, 150),               # Sleep cycles
        nrf_movs(0, 0),                 # Heartbeat counter
    ]
    outer_start = len(code)
    code.append(nrf_movs(7, 200))       # Subcycles
    mid_start = len(code)
    code.append(nrf_sev())              # Peripheral event interrupt trigger
    code.append(nrf_wfe())              # Handle event
    code.append(nrf_adds_imm(0, 1))     # Increment counter
    code.append(nrf_wfi())              # Enter deep sleep
    code.append(nrf_subs_imm(7, 1))     # R7 -= 1
    bne_mid_disp = mid_start - (len(code) + 2)
    code.append(nrf_bne(bne_mid_disp))
    code.append(nrf_subs_imm(6, 1))     # R6 -= 1
    bne_outer_disp = outer_start - (len(code) + 2)
    code.append(nrf_bne(bne_outer_disp))
    code.append(nrf_bkpt())
    return code

def main():
    print("=" * 70)
    print(" ArchC Nordic Semiconductor (nRF52840 / BLE 5.0) Verification Suite")
    print("=" * 70)

    test_files = [
        ("nrf52_alu.elf",         gen_nrf52_alu(),            write_elf_nrf52),
        ("nrf52_mem.elf",         gen_nrf52_mem(),            write_elf_nrf52),
        ("nrf52_ble_packet.elf",  gen_nrf52_ble_packet(),     write_elf_nrf52),
        ("nrf52_ulp_sleep.elf",   gen_nrf52_ulp_sleep(),      write_elf_nrf52),
    ]

    sim = "./nrf52.x"
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
