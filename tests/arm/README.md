# ArchC ARM Architecture Model & Verification Suite

This directory contains a complete ARM (ARMv7 / AArch32) architecture description and test suite modeled in ArchC and SystemC.

## Architecture Model Files
- `arm.ac`: Processor declaration (16 registers, CPSR, 512MB memory, 32-bit wordsize, little-endian).
- `arm.isa`: Instruction set architecture (Data Processing, Memory, Branch, System/Halt formats).
- `arm_isa.cpp`: Behavior definitions with full condition code evaluation (`EQ`, `NE`, `CS`, `CC`, `MI`, `PL`, `VS`, `VC`, `HI`, `LS`, `GE`, `LT`, `GT`, `LE`, `AL`).
- `arm_test_gen.py`: Test generator creating synthetic HEX and ELF32/ELF64 workloads.

## Building and Running

```bash
# 1. Generate simulator
../../install_local/bin/acsim arm.ac -nw -nci

# 2. Build simulator
make -f Makefile.archc -j$(sysctl -n hw.ncpu)

# 3. Run test generator and verification suite
python3 arm_test_gen.py
```
