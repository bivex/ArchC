# ArchC ESP32-C3 (Single-Core 32-bit RISC-V RV32IMC) Model & Verification Suite

This directory contains a complete processor model for the ESP32-C3 microcontroller powered by the 32-bit RISC-V RV32IMC core.

## Features
- 32-bit RISC-V Core (`ac_wordsize 32`, `ac_fetchsize 32`, Little-Endian).
- 32 32-bit General Purpose Registers (`x0`–`x31`, `x0` hardwired to 0).
- Standard RV32I base instructions + RV32M Hardware Multiply & Divide extension (`mul`, `mulh`, `mulhu`, `div`, `divu`, `rem`, `remu`).
- Control and status register modeling (`mstatus`, `mie`, `mtvec`, `mepc`, `mcause`).
- Native Little-Endian ELF32 (`EM_RISCV = 243 = 0xF3`) loader support.
- Simulation speed: **500+ MIPS**.
