#!/usr/bin/env python3
"""
ArchC Performance & Bottleneck Analysis Tool
Benchmarks .x simulator binaries across diverse workloads and optimization configurations.
"""

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
def nop(): return encode_r(0x00, 0, 0, 0, 0, 0x01)

def addi(rt, rs, imm): return encode_i(0x08, rs, rt, imm)
def andi(rt, rs, imm): return encode_i(0x0C, rs, rt, imm)
def ori(rt, rs, imm): return encode_i(0x0D, rs, rt, imm)
def lw(rt, rs, imm): return encode_i(0x23, rs, rt, imm)
def sw(rt, rs, imm): return encode_i(0x2B, rs, rt, imm)
def beq(rs, rt, imm): return encode_i(0x04, rs, rt, imm)
def bne(rs, rt, imm): return encode_i(0x05, rs, rt, imm)

def j(addr): return encode_j(0x02, addr)
def halt(): return encode_j(0x3F, 0)

def write_hex(filename, instructions, base_addr=0x1000):
    filepath = os.path.join(BENCH_DIR, filename)
    with open(filepath, "w") as f:
        f.write(f"0x{base_addr:08X} ")
        for instr in instructions:
            f.write(f"0x{instr:08X} ")
        f.write("\n")
    return filepath

def generate_workloads():
    # 1. ALU Intensive: 5,000,000 loop iterations * 8 instrs/iter = ~40,000,000 instrs
    alu_iter = 5000000
    alu_code = [
        addi(1, 0, (alu_iter >> 16) & 0xFFFF),
        sll(1, 1, 16),
        ori(1, 1, alu_iter & 0xFFFF),
        addi(2, 0, 1),
        addi(3, 0, 2),
        addi(4, 0, 3),
        addi(5, 0, 4),
        # Loop start (index 7)
        add(2, 2, 3),
        sub(3, 3, 4),
        and_op(4, 4, 5),
        or_op(5, 5, 2),
        xor_op(2, 2, 4),
        slt(6, 2, 3),
        addi(1, 1, -1),
        bne(1, 0, 7 - 14), # offset = -7
        halt()
    ]
    write_hex("workload_alu.hex", alu_code)

    # 2. Memory Intensive: 4,000,000 iterations * 6 instrs = ~24,000,000 instrs
    mem_iter = 4000000
    mem_code = [
        addi(1, 0, (mem_iter >> 16) & 0xFFFF),
        sll(1, 1, 16),
        ori(1, 1, mem_iter & 0xFFFF),
        addi(2, 0, 0x10000), # Mem buffer address
        addi(3, 0, 0xABCD),
        # Loop start (index 5)
        sw(3, 2, 0),
        lw(4, 2, 0),
        sw(4, 2, 4),
        lw(5, 2, 4),
        addi(1, 1, -1),
        bne(1, 0, 5 - 10), # offset = -5
        halt()
    ]
    write_hex("workload_mem.hex", mem_code)

    # 3. Branch Intensive: 4,000,000 iterations * 6 instrs = ~24,000,000 instrs
    br_iter = 4000000
    br_code = [
        addi(1, 0, (br_iter >> 16) & 0xFFFF),
        sll(1, 1, 16),
        ori(1, 1, br_iter & 0xFFFF),
        addi(2, 0, 0),
        # Loop start (index 4)
        andi(3, 1, 1),
        beq(3, 0, 3),     # If even -> skip to even (+3 instrs)
        addi(2, 2, 1),    # odd
        beq(0, 0, 2),     # jump to dec (+2 instrs)
        addi(2, 2, 2),    # even
        addi(1, 1, -1),   # dec
        bne(1, 0, 4 - 10), # loop
        halt()
    ]
    write_hex("workload_branch.hex", br_code)

    # 4. Mixed Real-World Kernel (Hash/Fibonacci): 3,000,000 iterations
    mix_iter = 3000000
    mix_code = [
        addi(1, 0, (mix_iter >> 16) & 0xFFFF),
        sll(1, 1, 16),
        ori(1, 1, mix_iter & 0xFFFF),
        addi(2, 0, 0),   # fib_a = 0
        addi(3, 0, 1),   # fib_b = 1
        addi(4, 0, 0x20000), # table
        # Loop start (index 6)
        add(5, 2, 3),    # c = a + b
        sw(5, 4, 0),     # table[0] = c
        lw(2, 4, 0),     # a = table[0]
        sub(3, 5, 3),    # b = c - b
        sll(5, 5, 2),    # c << 2
        xor_op(2, 2, 5), # a ^= (c << 2)
        addi(1, 1, -1),
        bne(1, 0, 6 - 13),
        halt()
    ]
    write_hex("workload_mixed.hex", mix_code)

def run_simulation(sim_path, hex_path):
    env = dict(os.environ)
    env["PKG_CONFIG_PATH"] = f"{INSTALL_PREFIX}/lib/pkgconfig:{env.get('PKG_CONFIG_PATH', '')}"

    start_time = time.perf_counter()
    p = subprocess.run([sim_path, f"--load={hex_path}"],
                       cwd=BENCH_DIR, env=env,
                       stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                       text=True)
    elapsed = time.perf_counter() - start_time
    output = p.stdout + p.stderr

    # Parse instruction count from ArchC statistics
    instr_count = 0
    m = re.search(r"Number of instructions executed:\s*(\d+)", output)
    if m:
        instr_count = int(m.group(1))

    mips = (instr_count / (elapsed * 1e6)) if elapsed > 0 else 0
    return {
        "elapsed": elapsed,
        "instructions": instr_count,
        "mips": mips,
        "exit_code": p.returncode,
        "output": output
    }

def rebuild_simulator(acsim_flags=""):
    """
    Regenerates and recompiles simulator with specific ArchC generator flags
    """
    env = dict(os.environ)
    env["PKG_CONFIG_PATH"] = f"{INSTALL_PREFIX}/lib/pkgconfig:{env.get('PKG_CONFIG_PATH', '')}"

    acsim_bin = os.path.join(INSTALL_PREFIX, "bin/acsim")
    cmd = f"{acsim_bin} bench32.ac {acsim_flags}"
    res = subprocess.run(cmd, shell=True, cwd=BENCH_DIR, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if res.returncode != 0:
        print(f"Error in acsim: {res.stderr}")
        return False

    res = subprocess.run("make clean && make -j4", shell=True, cwd=BENCH_DIR, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if res.returncode != 0:
        print(f"Error compiling Makefile: {res.stderr}")
        return False

    return True

def main():
    print("=" * 70)
    print(" ArchC Simulator Performance & Bottleneck Analysis Benchmark")
    print("=" * 70)

    generate_workloads()
    workloads = [
        ("ALU Intensive", "workload_alu.hex"),
        ("Memory Read/Write", "workload_mem.hex"),
        ("Branch & Control Flow", "workload_branch.hex"),
        ("Mixed Compute/Mem Kernel", "workload_mixed.hex")
    ]

    configs = [
        ("Default (Direct Threading + Decode Cache + O3)", ""),
        ("No Decode Cache (-ndc)", "-ndc"),
        ("No Direct Threading (-nt)", "-nt"),
        ("Full Decode Optimization (-fdc)", "-fdc"),
    ]

    all_results = {}

    for config_name, flags in configs:
        print(f"\n--- Testing Configuration: {config_name} ---")
        if not rebuild_simulator(flags):
            print(f"[!] Failed to build config: {config_name}")
            continue

        sim_path = os.path.join(BENCH_DIR, "bench32.x")
        config_res = {}

        for wl_name, wl_file in workloads:
            hex_file = os.path.join(BENCH_DIR, wl_file)
            # Run 3 trials and take the best
            trials = []
            for _ in range(3):
                res = run_simulation(sim_path, hex_file)
                if res["exit_code"] == 0 and res["instructions"] > 0:
                    trials.append(res)

            if trials:
                best = min(trials, key=lambda x: x["elapsed"])
                config_res[wl_name] = best
                print(f"  {wl_name:<25}: {best['instructions']:>10,} instrs | {best['elapsed']:>6.3f}s | {best['mips']:>7.2f} MIPS")
            else:
                print(f"  {wl_name:<25}: FAILED")

        all_results[config_name] = config_res

    # Summary table
    print("\n" + "=" * 80)
    print(f"{'Configuration':<45} | {'Workload':<20} | {'MIPS':<10} | {'Time (s)':<8}")
    print("=" * 80)
    for cfg, res in all_results.items():
        for wl, stats in res.items():
            print(f"{cfg:<45} | {wl:<20} | {stats['mips']:>8.2f}   | {stats['elapsed']:>6.3f}s")

if __name__ == "__main__":
    main()
