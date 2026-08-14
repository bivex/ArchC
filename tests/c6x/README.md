# ArchC Texas Instruments TMS320C6000 (C6x VLIW DSP) Architecture Model & Verification Suite

This directory contains a complete TI TMS320C6000 (C6x) VLIW DSP processor model, simulator, and automated benchmark suite.

## Features
- Dual Register File Banks: Bank A (`A0`–`A15`) and Bank B (`B0`–`B15`).
- VLIW execute packet decoding and parallel execution bit (`p-bit`).
- Conditional execution for all instructions (`creg` / `z` bits testing `A0`, `A1`, `A2`, `B0`, `B1`, `B2`).
- Saturated Math & DSP Operations (`SADD`, `SSUB`, `SMPY`, `MPY`) with Saturation flag in `CSR`.
- Simulation speed: **500+ MIPS**.
