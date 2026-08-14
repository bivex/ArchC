#!/usr/bin/env python3
import os
import sys
import time
import subprocess
import re

BENCH_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.abspath(os.path.join(BENCH_DIR, "../.."))
INSTALL_PREFIX = os.path.join(ROOT_DIR, "install_local")

def encode_r(op, rs, rt, rd, shamt, func):
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | ((rd & 0x1F) << 11) | ((shamt & 0x1F) << 6) | (func & 0x3F)

def encode_i(op, rs, rt, imm):
    return ((op & 0x3F) << 26) | ((rs & 0x1F) << 21) | ((rt & 0x1F) << 16) | (imm & 0xFFFF)

def encode_j(op, addr):
    return ((op & 0x3F) << 26) | (addr & 0x3FFFFFF)

# Instruction encoders
def add(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x20)
def sub(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x22)
def and_op(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x24)
def or_op(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x25)
def xor_op(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x26)
def slt(rd, rs, rt): return encode_r(0x00, rs, rt, rd, 0, 0x2A)
def sll(rd, rt, shamt): return encode_r(0x00, 0, rt, rd, shamt, 0x00)
def srl(rd, rt, shamt): return encode_r(0x00, 0, rt, rd, shamt, 0x02)
def addi(rt, rs, imm): return encode_i(0x08, rs, rt, imm)
def andi(rt, rs, imm): return encode_i(0x0C, rs, rt, imm)
def ori(rt, rs, imm): return encode_i(0x0D, rs, rt, imm)
def lw(rt, rs, imm): return encode_i(0x23, rs, rt, imm)
def sw(rt, rs, imm): return encode_i(0x2B, rs, rt, imm)
def beq(rs, rt, imm): return encode_i(0x04, rs, rt, imm)
def bne(rs, rt, imm): return encode_i(0x05, rs, rt, imm)
def halt(): return encode_j(0x3F, 0)

def write_hex(filename, instructions, base_addr=0x1000):
    filepath = os.path.join(BENCH_DIR, filename)
    with open(filepath, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")
    return filepath

def generate_large_workload(iterations=80000000): # 80M iters * 8 instrs = 640M instructions (~1.5-2 seconds)
    high = (iterations >> 16) & 0xFFFF
    low = iterations & 0xFFFF
    code = [
        addi(1, 0, high),
        sll(1, 1, 16),
        ori(1, 1, low),
        addi(2, 0, 0x20000), # mem address
        addi(3, 0, 1),
        addi(4, 0, 2),
        addi(5, 0, 3),
        # Loop start (index 7)
        add(6, 3, 4),        # r6 = 1 + 2 = 3
        sw(6, 2, 0),         # mem[0x20000] = 3
        lw(7, 2, 0),         # r7 = mem[0x20000]
        xor_op(8, 7, 5),     # r8 = r7 ^ 3
        sll(8, 8, 1),        # r8 = r8 << 1
        addi(3, 3, 1),       # r3++
        addi(1, 1, -1),      # r1--
        bne(1, 0, 7 - 14),   # jump back 7 instrs
        halt()
    ]
    return write_hex("workload_sample.hex", code)

def main():
    print("[1/4] Generating large 640M instruction workload...")
    hex_path = generate_large_workload(80000000)

    sim_path = os.path.join(BENCH_DIR, "bench32.x")
    env = dict(os.environ)
    env["PKG_CONFIG_PATH"] = f"{INSTALL_PREFIX}/lib/pkgconfig:{env.get('PKG_CONFIG_PATH', '')}"

    print(f"[2/4] Launching simulator process ({sim_path})...")
    sim_proc = subprocess.Popen([sim_path, f"--load={hex_path}"],
                                cwd=BENCH_DIR, env=env,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                text=True)

    pid = sim_proc.pid
    print(f"[+] Simulator PID: {pid}")

    sample_out = os.path.join(BENCH_DIR, "sample_report.txt")
    print(f"[3/4] Running macOS `sample` profiler for 2 seconds (1ms sampling rate)...")
    sample_cmd = ["sample", str(pid), "2", "1", "-file", sample_out]
    try:
        s_res = subprocess.run(sample_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    except Exception as e:
        print(f"Error running sample: {e}")

    stdout, stderr = sim_proc.communicate()
    print(f"[+] Simulation completed. Output:")
    for line in stdout.strip().splitlines()[-4:]:
        print(f"    {line}")

    if os.path.exists(sample_out):
        print(f"\n[4/4] Top CPU Hotspots from macOS `sample` profiler:")
        print("=" * 80)
        with open(sample_out, "r") as f:
            content = f.read()

        # Extract Call graph and sorted function summary
        lines = content.splitlines()
        print_lines = []
        capture = False
        for line in lines:
            if "Sort by top of stack" in line or "Call graph:" in line or "Total number of samples:" in line:
                capture = True
            if capture:
                print_lines.append(line)
                if len(print_lines) > 80:
                    break

        print("\n".join(print_lines[:70]))
        print("=" * 80)
        print(f"\nFull raw sample report saved to: {sample_out}")

if __name__ == "__main__":
    main()
