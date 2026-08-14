# ArchC DEC Alpha (Alpha 21264 / AXP) Architecture Model & Verification Suite

This directory contains a complete 64-bit DEC Alpha processor model, simulator, and automated benchmark suite.

## Features
- 64-bit pure RISC architecture (`ac_wordsize 64`, `ac_fetchsize 32`).
- 32 64-bit General-Purpose Registers (`$0`–`$31`), `$31` (`$zero`) hardwired to 0.
- Conditional Moves (`CMOVEQ`, `CMOVNE`) without condition code registers.
- Native Little-Endian 64-bit ELF (`ELFCLASS64`, `EM_ALPHA = 0x9026`) loader support.
- Simulation speed: **500+ MIPS**.
