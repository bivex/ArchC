# ArchC Atmel AVR (ATmega328P / Arduino) Architecture Model & Verification Suite

This directory contains a complete 8-bit Atmel AVR (ATmega328P) processor model, simulator, and automated benchmark suite.

## Features
- 8-bit Harvard RISC microcontroller (`ac_wordsize 8`, `ac_fetchsize 16`, Little-Endian).
- 32 8-bit General-Purpose Working Registers (`R0`–`R31`).
- 16-bit Pointer Register Pairs: `X` (`R27:R26`), `Y` (`R29:R28`), `Z` (`R31:R30`).
- Status Register `SREG` (`I`, `T`, `H`, `S`, `V`, `N`, `Z`, `C`).
- Indirect Data Memory SRAM access via pointer registers.
- Native Little-Endian ELF32 (`EM_AVR = 83`) loader support.
