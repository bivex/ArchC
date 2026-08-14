# ArchC Intel 8051 (8-bit Harvard) Architecture Model & Verification Suite

This directory contains a complete Intel 8051 microcontroller model, simulator, and automated verification suite.

## Features
- 8-bit registers (`ACC`, `B`, `PSW`, `SP`, `DPTR`, `R0`..`R7`).
- Harvard architecture (separate Program Memory `PM` and Data Memory `DM`).
- Variable-length instructions (1-byte, 2-byte, 3-byte).
- Support for `DJNZ` decrement-and-jump loop constructs.
