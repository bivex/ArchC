# ArchC Motorola 68000 (m68k) Architecture Model & Verification Suite

This directory contains a complete Motorola 68000 (m68k) CISC processor model, simulator, and automated benchmark suite.

## Features
- 32-bit linear address space and registers (`ac_wordsize 32`, `ac_fetchsize 16`, Big-Endian).
- 8 32-bit Data Registers (`D0`–`D7`) and 8 32-bit Address Registers (`A0`–`A7`).
- Status Register `SR` / Condition Code Register `CCR` (`X`, `N`, `Z`, `V`, `C`).
- Native Big-Endian ELF32 (`EM_68K = 4`) loader support.
