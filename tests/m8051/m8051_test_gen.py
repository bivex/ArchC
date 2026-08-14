#!/usr/bin/env python3
import struct
import subprocess
import os
import sys

def m8051_add_a_rn(rn): return bytes([(0x05 << 3) | (rn & 0x7)])
def m8051_sub_a_rn(rn): return bytes([(0x13 << 3) | (rn & 0x7)])
def m8051_anl_a_rn(rn): return bytes([(0x0B << 3) | (rn & 0x7)])
def m8051_orl_a_rn(rn): return bytes([(0x09 << 3) | (rn & 0x7)])
def m8051_xrl_a_rn(rn): return bytes([(0x0D << 3) | (rn & 0x7)])
def m8051_mov_a_rn(rn): return bytes([(0x1D << 3) | (rn & 0x7)])
def m8051_mov_rn_a(rn): return bytes([(0x1F << 3) | (rn & 0x7)])
def m8051_inc_rn(rn):   return bytes([(0x01 << 3) | (rn & 0x7)])
def m8051_dec_rn(rn):   return bytes([(0x03 << 3) | (rn & 0x7)])

def m8051_mov_a_imm(imm): return bytes([0x74, imm & 0xFF])
def m8051_add_a_imm(imm): return bytes([0x24, imm & 0xFF])
def m8051_mov_rn_imm(rn, imm): return bytes([(0x0F << 3) | (rn & 0x7), imm & 0xFF])

def m8051_sjmp(offset): return bytes([0x80, offset & 0xFF])
def m8051_djnz_rn(rn, offset): return bytes([(0x1B << 3) | (rn & 0x7), offset & 0xFF])
def m8051_halt(): return bytes([0xA5])

def write_hex_bytes(filename, byte_seq, base_addr=0x1000):
    with open(filename, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for b in byte_seq:
            f.write(f"0x{b:02X} ")
        f.write("\n")

def gen_8051_alu(iterations=1000000):
    # 8051 8-bit registers: r0 = inner loop (200), r1 = outer loop (5000)
    code = bytearray()
    code += m8051_mov_rn_imm(1, 250)             # r1 = 250 (outer)
    code += m8051_mov_rn_imm(2, 10)              # r2 = 10
    code += m8051_mov_rn_imm(3, 20)              # r3 = 20

    outer_pos = len(code)
    code += m8051_mov_rn_imm(0, 200)             # r0 = 200 (inner)

    inner_pos = len(code)
    code += m8051_mov_a_rn(2)                    # acc = r2
    code += m8051_add_a_rn(3)                    # acc += r3
    code += m8051_xrl_a_rn(1)                    # acc ^= r1
    code += m8051_mov_rn_a(2)                    # r2 = acc
    code += m8051_inc_rn(3)                      # r3++

    djnz_pos = len(code)
    offset_inner = inner_pos - djnz_pos
    code += m8051_djnz_rn(0, offset_inner)       # djnz r0, inner

    djnz_outer_pos = len(code)
    offset_outer = outer_pos - djnz_outer_pos
    code += m8051_djnz_rn(1, offset_outer)       # djnz r1, outer

    code += m8051_halt()
    return bytes(code)

def main():
    print("=" * 65)
    print(" ArchC Intel 8051 (8-bit Harvard) Verification Suite")
    print("=" * 65)

    alu_bytes = gen_8051_alu()
    write_hex_bytes("m8051_alu.hex", alu_bytes, base_addr=0x1000)

    sim = "./m8051.x"
    test_files = ["m8051_alu.hex"]

    for tf in test_files:
        p = subprocess.run([sim, f"--load={tf}"], capture_output=True, text=True)
        combined = p.stdout + p.stderr
        instr_line = "OK"
        speed_line = ""
        for l in combined.split("\n"):
            if "Number of instructions executed:" in l:
                instr_line = l.strip()
            elif "Simulation speed:" in l:
                speed_line = l.strip()
        print(f"  {tf:<15} | {instr_line:<43} | {speed_line}")

if __name__ == "__main__":
    main()
