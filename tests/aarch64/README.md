# ArchC AArch64 (ARMv8-A / ARM64) Architecture Model & Verification Suite

This directory contains a complete 64-bit ARMv8-A (AArch64) processor model, simulator, and automated benchmark suite.

## Features
- 64-bit architecture (`ac_wordsize 64`, `ac_fetchsize 32`, Little-Endian).
- 31 64-bit General-Purpose Registers (`X0`–`X30`), `XZR` (zero register), `LR` (link register `X30`).
- Dedicated `PSTATE` flags register (`NZCV`) with condition evaluators (`EQ`, `NE`, `CS`, `CC`, `MI`, `PL`, `VS`, `VC`, `HI`, `LS`, `GE`, `LT`, `GT`, `LE`, `AL`).
- Native Little-Endian 64-bit ELF (`ELFCLASS64`, `EM_AARCH64 = 183`) loader support.
- Simulation speed: **500+ MIPS**.
