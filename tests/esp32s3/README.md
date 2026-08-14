# ArchC ESP32-S3 (Tensilica Xtensa LX7 + Vector AI PIE) Model & Verification Suite

This directory contains a complete processor model for the ESP32-S3 dual-core microcontroller featuring the Tensilica Xtensa LX7 core and custom AI Vector Extensions (PIE).

## Features
- 32-bit Xtensa LX7 Core (`ac_wordsize 32`, `ac_fetchsize 32`, Little-Endian).
- 16 32-bit General-Purpose Registers (`A0`–`A15`).
- 8 64-bit SIMD Vector Registers (`Q0`–`Q7`) for TinyML neural network inference.
- AI Vector Acceleration (PIE instructions):
  - `ee.vadd.s16`: Packed 16-bit vector addition
  - `ee.vmul.s16`: Packed 16-bit vector multiplication
  - `ee.vdot.s8`: Int8 quantized neural network dot-product / GEMM
  - `ee.vld.q` / `ee.vst.q`: 64-bit vector load/store
- Native Little-Endian ELF32 (`EM_XTENSA = 94`) loader support.
- Simulation speed: **500+ MIPS**.
