#!/usr/bin/env python3
import subprocess
import os
import sys

def main():
    root = os.path.dirname(os.path.abspath(__file__))
    archs = [
        ("bench32", "bench_gen.py"),
        ("arm",     "arm_test_gen.py"),
        ("riscv",   "riscv_test_gen.py"),
        ("mips",    "mips_test_gen.py"),
        ("sparc",   "sparc_test_gen.py"),
        ("powerpc", "powerpc_test_gen.py"),
        ("i386",    "i386_test_gen.py"),
        ("x86_64",  "x86_64_test_gen.py"),
        ("aarch64", "aarch64_test_gen.py"),
        ("alpha",   "alpha_test_gen.py"),
        ("c6x",     "c6x_test_gen.py"),
        ("esp32",   "esp32_test_gen.py"),
        ("esp32s3", "esp32s3_test_gen.py"),
        ("esp32c3", "esp32c3_test_gen.py"),
        ("stm32",     "stm32_test_gen.py"),
        ("nrf52",     "nrf52_test_gen.py"),
        ("apple_arm", "apple_arm_test_gen.py"),
        ("m68k",      "m68k_test_gen.py"),
        ("m6502",     "m6502_test_gen.py"),
        ("avr",       "avr_test_gen.py"),
        ("m8051",     "m8051_test_gen.py"),
    ]

    print("=" * 72)
    print("        ArchC Complete Multi-Architecture Verification Suite")
    print("=" * 72)

    all_passed = True
    for arch, script in archs:
        arch_dir = os.path.join(root, arch)
        script_path = os.path.join(arch_dir, script)
        if not os.path.exists(script_path):
            print(f"[-] Missing {script_path}")
            all_passed = False
            continue

        print(f"\n>>> Running {arch.upper()} Test Suite ({script})...")
        p = subprocess.run([sys.executable, script], cwd=arch_dir, capture_output=True, text=True)
        if p.returncode != 0:
            print(f"FAILED {arch}: {p.stderr}")
            all_passed = False
        else:
            print(p.stdout.strip())

    print("\n" + "=" * 72)
    if all_passed:
        print(" [✓] ALL 20 ARCHITECTURES (ARM, RISC-V, MIPS, SPARC, PPC, x86, x86-64, AArch64, Apple Silicon ARM64e, ALPHA, C6X, ESP32, ESP32-S3, ESP32-C3, STM32, Nordic nRF52, m68k, MOS 6502, AVR, 8051) PASSED 100%!")
    else:
        print(" [X] SOME TESTS FAILED")
    print("=" * 72)

if __name__ == "__main__":
    main()
