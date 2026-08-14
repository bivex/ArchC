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
        ("m8051",   "m8051_test_gen.py"),
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
        print(" [✓] ALL 8 ARCHITECTURES (ARM, RISC-V, MIPS, SPARC, PPC, x86, 8051, BENCH32) PASSED 100%!")
    else:
        print(" [X] SOME TESTS FAILED")
    print("=" * 72)

if __name__ == "__main__":
    main()
