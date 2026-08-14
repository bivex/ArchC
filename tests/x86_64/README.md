# ArchC Intel x86-64 (AMD64 / x64) Architecture Model & Verification Suite

This directory contains a complete 64-bit Intel x86-64 (AMD64 / x64) CISC processor model, simulator, and automated benchmark suite.

## Features
- 64-bit Word Size (`ac_wordsize 64`, `ac_fetchsize 32`).
- 16 64-bit General-Purpose Registers (`RAX`, `RCX`, `RDX`, `RBX`, `RSP`, `RBP`, `RSI`, `RDI`, `R8`–`R15`).
- Flags register (`RFLAGS`: ZF, CF, SF, OF).
- Native Little-Endian 64-bit ELF (`ELFCLASS64`, `EM_X86_64 = 62`) loader support.
- Simulation speed: **583+ MIPS** on ALU and **400+ MIPS** on Memory workloads.
