#!/usr/bin/env python3
import os
import subprocess

TESTS = [
    ("ARM (ARMv7)", "tests/arm", "arm_test_gen.py"),
    ("RISC-V (RV32I/64I)", "tests/riscv", "riscv_test_gen.py"),
    ("MIPS (MIPS32)", "tests/mips", "mips_test_gen.py"),
    ("SPARC (SPARC V8)", "tests/sparc", "sparc_test_gen.py"),
    ("PowerPC (PPC32)", "tests/powerpc", "powerpc_test_gen.py"),
    ("Intel x86 (i386)", "tests/i386", "i386_test_gen.py"),
    ("Intel x86-64 (AMD64)", "tests/x86_64", "x86_64_test_gen.py"),
    ("AArch64 (ARM64)", "tests/aarch64", "aarch64_test_gen.py"),
    ("DEC Alpha (21264)", "tests/alpha", "alpha_test_gen.py"),
    ("TI C6x (VLIW DSP)", "tests/c6x", "c6x_test_gen.py"),
    ("ESP32 (Xtensa LX6)", "tests/esp32", "esp32_test_gen.py"),
    ("ESP32-S3 (Vector AI)", "tests/esp32s3", "esp32s3_test_gen.py"),
    ("ESP32-C3 (RISC-V)", "tests/esp32c3", "esp32c3_test_gen.py"),
    ("STM32F103 (Cortex-M3)", "tests/stm32", "stm32_test_gen.py"),
    ("Nordic nRF52 (BLE/ULP)", "tests/nrf52", "nrf52_test_gen.py"),
    ("Motorola 68000 (m68k)", "tests/m68k", "m68k_test_gen.py"),
    ("MOS 6502 (NES/Apple)", "tests/m6502", "m6502_test_gen.py"),
    ("Atmel AVR (Arduino)", "tests/avr", "avr_test_gen.py"),
    ("Intel 8051 (Harvard)", "tests/m8051", "m8051_test_gen.py"),
]

root = "/Volumes/External/Code/ArchC"
env = os.environ.copy()
env["PKG_CONFIG_PATH"] = f"{root}/install_local/lib/pkgconfig:{env.get('PKG_CONFIG_PATH', '')}"

print(f"{'Архитектура':<25} | {'Тестовый бинарник':<22} | {'Инструкций':<14} | {'Скорость (MIPS)':<18}")
print("-" * 86)

for name, rel_dir, script in TESTS:
    cwd = os.path.join(root, rel_dir)
    res = subprocess.run(["python3", script], cwd=cwd, capture_output=True, text=True, env=env)
    out = res.stdout + "\n" + res.stderr
    
    for line in out.splitlines():
        line = line.strip()
        if "|" in line and ("Number of instructions" in line or "instr/s" in line):
            parts = [p.strip() for p in line.split("|")]
            bin_name = parts[0]
            inst_str = parts[1].replace("Number of instructions executed:", "").strip()
            speed_str = parts[2].replace("Simulation speed:", "").strip()
            
            # Format speed to MIPS
            if "K instr/s" in speed_str:
                k_val = float(speed_str.replace("K instr/s", "").strip())
                mips_val = f"{k_val / 1000.0:.2f} MIPS"
            else:
                mips_val = speed_str
                
            print(f"{name:<25} | {bin_name:<22} | {inst_str:<14} | {mips_val:<18}")
