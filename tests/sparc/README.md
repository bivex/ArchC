# ArchC SPARC (SPARC V8) Architecture Model & Verification Suite

This directory contains a complete SPARC V8 processor model, simulator, and automated benchmark suite.

## Features
- 32 registers (`%g0`..`%g7`, `%o0`..`%o7`, `%l0`..`%l7`, `%i0`..`%i7`), `%g0` hardwired to 0.
- Condition codes in `%psr` (icc: N, Z, V, C).
- Big-Endian 32-bit architecture with ELF32 and ELF64 loader support.
- Throughput: **583+ MIPS**.
