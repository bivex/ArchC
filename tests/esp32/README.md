# ArchC ESP32 DevKit V1 (Tensilica Xtensa LX6 / ESP-WROOM-32) Model & Verification Suite

This directory contains a complete Tensilica Xtensa LX6 processor model (used in ESP32 DevKit V1 / ESP-WROOM-32), simulator, and automated benchmark suite.

## Features
- 32-bit Harvard RISC processor (`ac_wordsize 32`, `ac_fetchsize 32`, Little-Endian).
- 16 General-Purpose Physical Registers (`A0`–`A15`, `A1` mapped to `SP`).
- Special Control Registers: `SAR` (Shift Amount), `LCOUNT`/`LBEG`/`LEND` (Zero-overhead hardware loop), `PS` (Processor Status).
- High performance arithmetic (`mull`, `add`, `sub`, `and`, `or`, `xor`, `movi`, `addi`, `addmi`).
- Memory load/store (`l32i`, `s32i`).
- Native Little-Endian ELF32 (`EM_XTENSA = 94 = 0x5E`) loader support.
- Simulation speed: **500+ MIPS**.
