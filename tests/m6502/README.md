# ArchC MOS Technology 6502 (NES / Apple II) Architecture Model & Verification Suite

This directory contains a complete 8-bit MOS Technology 6502 processor model, simulator, and automated benchmark suite.

## Features
- 8-bit architecture (`ac_wordsize 8`, `ac_fetchsize 8`).
- Accumulator `A`, Index Registers `X` and `Y`, Stack Pointer `S`, Status Register `P` (`N`, `V`, `B`, `D`, `I`, `Z`, `C`).
- Zero-Page addressing mode (`sta_zp`, `lda_zp`, `ldx_zp`, `stx_zp`, `ldy_zp`, `sty_zp`).
- Variable-length instructions (1-byte Implied, 2-byte Immediate/Zero-Page/Branch, 3-byte Absolute).
- Fast simulation speed.
